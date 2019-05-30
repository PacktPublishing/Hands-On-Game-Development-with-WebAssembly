#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_opengl.h>

#include <GLES3/gl3.h>
#include <stdlib.h>
#include <emscripten.h>

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600
#define FLOAT32_BYTE_SIZE 4
#define STRIDE FLOAT32_BYTE_SIZE*4

const GLchar* vertex_shader_code[] = {
      "precision mediump float; \n"

      "attribute vec4 a_position;   \n"
      "attribute vec2 a_texcoord;   \n"

      "varying vec2 v_texcoord; \n"

      "void main() {    \n"
        "gl_Position = a_position;    \n"
        "v_texcoord = a_texcoord;   \n"
      "}    \n"

};

const GLchar* fragment_shader_code[] = {
      "precision mediump float; \n"

      "varying vec2 v_texcoord; \n"

      "uniform sampler2D u_texture; \n"
      "uniform sampler2D u_normal; \n"

      "uniform vec3 u_light_pos;   \n"

      "const float ambient = 0.6;   \n"                 
      "const float specular = 32.0;   \n"

      "const vec3 view_pos = vec3(400, 300,-100);   \n"
      "const vec4 light_color = vec4( 0.6, 0.6, 0.6, 0.0); \n"

      "void main() {    \n"                             
        "vec4 tex = texture2D(u_texture, v_texcoord);   \n"
        "vec4 ambient_frag = tex * ambient; \n"

        "ambient_frag.rgb *= light_color.rgb;   \n"

        "vec3 norm = vec3(texture2D(u_normal, v_texcoord)); \n"
        "norm.xyz *= 2.0;   \n"
        "norm.xyz -= 1.0;   \n"
        "vec3 light_dir = normalize(gl_FragCoord.xyz - u_light_pos);  \n"

        "vec3 view_dir = normalize(view_pos - gl_FragCoord.xyz);    \n"
        "vec3 reflect_dir = reflect(light_dir, norm);  \n"

        "float reflect_dot = max( dot(view_dir, reflect_dir), 0.0 );  \n"
        "float spec = pow(reflect_dot, specular);    \n"
        "vec4 specular_frag = spec * light_color;   \n" 

        "float diffuse = max(dot(norm, light_dir), 0.0);   \n"         
        "vec4 diffuse_frag = vec4(diffuse * light_color.r, diffuse * light_color.g, diffuse * light_color.b, 0.0);  \n"
        "gl_FragColor = ambient_frag + diffuse_frag + specular_frag;   \n"
      "}    \n"
};

GLuint program = 0;
GLint sdl_program = 0;
GLuint circle_tex, normal_tex, light_tex;
GLuint normal_map;

GLint a_texcoord_location = -1;
GLint a_position_location = -1;
GLint u_texture_location = -1;
GLint u_normal_location = -1;
GLint u_light_pos_location = -1;

GLint u_translate_location = -1;
GLuint vertex_texture_buffer;

float vertex_texture_data[] = {
//  x,      y,     u,   v
    0.167,  0.213, 1.0, 1.0,
   -0.167,  0.213, 0.0, 1.0,
    0.167, -0.213, 1.0, 0.0,
   -0.167, -0.213, 0.0, 0.0,
   -0.167,  0.213, 0.0, 1.0,
    0.167, -0.213, 1.0, 0.0
};

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture* light_texture;

SDL_Surface* surface;

int light_width;
int light_height;

int light_x = 600; 
int light_y = 200;  
int light_z = -300;  

void game_loop();
void input();
void draw_light_icon();

int main() {
    SDL_Init( SDL_INIT_VIDEO );

    SDL_CreateWindowAndRenderer( CANVAS_WIDTH, CANVAS_HEIGHT, 0, &window, &renderer );

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(	vertex_shader,
                    1,
                    vertex_shader_code,
                    0);

    glCompileShader(vertex_shader);

    GLint compile_success = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_success);

    if(compile_success == GL_FALSE)
    {
        printf("failed to compile vertex shader\n");
        glDeleteShader(vertex_shader);
        return 0;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(	fragment_shader,
                    1,
                    fragment_shader_code,
                    0);

    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_success);

    if(compile_success == GL_FALSE)
    {
        printf("failed to compile fragment shader\n");

        GLint maxLength = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* errorLog = malloc(maxLength);
        glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &errorLog[0]);
        printf("error: %s\n", errorLog);

        glDeleteShader(fragment_shader);
        return 0;
    }

    program = glCreateProgram();
    glAttachShader(	program,
 	                vertex_shader);

    glAttachShader(	program,
 	                fragment_shader);

    glLinkProgram(program);

    GLint link_success = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &link_success);

    if (link_success == GL_FALSE)
    {
        printf("failed to link program\n");
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(program);

    u_texture_location = glGetUniformLocation(program, "u_texture");
    u_normal_location = glGetUniformLocation(program, "u_normal");
    u_light_pos_location = glGetUniformLocation(program, "u_light_pos");
    u_translate_location = glGetUniformLocation(program, "u_translate");

    a_position_location = glGetAttribLocation(program, "a_position");
    a_texcoord_location = glGetAttribLocation(program, "a_texcoord");

    glGenBuffers(1, &vertex_texture_buffer);

    glBindBuffer( GL_ARRAY_BUFFER, vertex_texture_buffer );
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_data), 
                 vertex_texture_data, GL_STATIC_DRAW);

    glGenTextures( 1,
                   &circle_tex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, circle_tex);

    surface = IMG_Load( "/sprites/circle.png" );

    if( !surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }

    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                128,    // sprite width
                128,    // sprite height
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                surface );

    glUniform1i(u_texture_location, 1);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface( surface );

    glGenTextures( 1,
                   &normal_tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_tex);

    surface = IMG_Load( "/sprites/ball-normal.png" );

    if( !surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }

    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                128,    // sprite width
                128,    // sprite height
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                surface );

    glUniform1i(u_normal_location, 1);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface( surface );

    surface = IMG_Load( "/sprites/light.png" );

    if( !surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }

    light_texture = SDL_CreateTextureFromSurface( renderer, surface );

    if( !light_texture ) {
        printf("failed to create light texture: %s\n", IMG_GetError() );
        return 0;
    }

    SDL_QueryTexture( light_texture, 
                        NULL, NULL, 
                        &light_width, &light_height );

    SDL_FreeSurface( surface );
// -- END LIGHT TEXTURE ---

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    emscripten_set_main_loop(game_loop, 0, 0);
}

void game_loop() {
    input();

    glGetIntegerv(GL_CURRENT_PROGRAM,&sdl_program);

    glUseProgram(program);

    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    glBindBuffer(GL_ARRAY_BUFFER, vertex_texture_buffer);
    glVertexAttribPointer(
        a_position_location,	// set up the a_position attribute
        2,                 		// how many attributes in the position
        GL_FLOAT,           	// data type of float
        GL_FALSE,           	// the data is not normalized
        4 * sizeof(float),  	// stride (how many array items until the next position)
        (void*)0            	// starting point for attribute
    );

    glEnableVertexAttribArray(a_texcoord_location);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_texture_buffer);
    glVertexAttribPointer(
        a_texcoord_location,	// set up the a_texcoord attribute
        2,                  	// how many attributes in the texture coordinates
        GL_FLOAT,           	// data type of float
        GL_FALSE,           	// the data is not normalized
        4 * sizeof(float),  	// stride (how many array items until the next position)
        (void*)(2 * sizeof(float)) 	// starting point for attribute
    );

    glUniform3f( u_light_pos_location, 
                (float)(light_x), (float)(600-light_y), (float)(light_z) );

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(sdl_program);
    draw_light_icon();
}

void input() {
    SDL_Event event;
    static int mouse_down = 0;

    if(SDL_PollEvent( &event ) ) 
	{
		if(event.type == SDL_MOUSEWHEEL ) 
		{
            if( event.wheel.y > 0 ) {
                light_z+= 100;
            }
            else {
                light_z-=100;
            }

            if( light_z > 10000 ) {
                light_z = 10000;
            }
            else if( light_z < -10000 ) {
                light_z = -10000;
            }
        }
		else if(event.type == SDL_MOUSEMOTION ) 
        {
            if( mouse_down == 1 ) {
                SDL_GetMouseState( &light_x, &light_y );
            }
        }
		else if(event.type == SDL_MOUSEBUTTONDOWN ) 
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                SDL_GetMouseState( &light_x, &light_y );
                mouse_down = 1;
            }
        }
		else if(event.type == SDL_MOUSEBUTTONUP ) 
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                mouse_down = 0;
            }
        }
    }
}

void draw_light_icon() { 
    SDL_Rect dest;
    dest.x = light_x - light_width / 2 - 32;
    dest.y = light_y - light_height / 2;
    dest.w = light_width;
    dest.h = light_height;

    SDL_RenderCopy( renderer, light_texture, NULL, &dest );
}
