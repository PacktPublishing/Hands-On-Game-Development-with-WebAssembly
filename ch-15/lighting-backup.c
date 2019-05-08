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

      "uniform vec3 u_light_pos;   \n"
      "uniform sampler2D u_texture; \n"
      "uniform sampler2D u_normal; \n"

      "const float ambient = 0.5;   \n"                 
      "const float specular = 0.8;   \n"

//      "const vec3 light_pos = vec3(200, 200, 50);   \n"
//      "const vec3 view_pos = vec3(400, 300, 10);   \n"
      "const vec3 view_pos = vec3(400, 300, 100);   \n"
      "const vec4 light_color = vec4( 1.0, 1.0, 1.0, 0.0); \n"

      "void main() {    \n"                             
        "vec4 tex = texture2D(u_texture, v_texcoord);   \n"
//        "vec4 tex = texture2D(u_normal, v_texcoord);   \n"
        "vec4 ambient_frag = tex * ambient; \n"

        "ambient_frag.rgb *= light_color.rgb;   \n"

        "vec3 norm = vec3(texture2D(u_normal, v_texcoord)); \n"
        "norm.xyz *= 2.0;   \n"
        "norm.xyz -= 1.0;   \n"
        "vec3 light_dir = normalize(gl_FragCoord.xyz - u_light_pos);  \n"

        "vec3 view_dir = normalize(gl_FragCoord.xyz - view_pos);    \n"
        "vec3 reflect_dir = reflect(light_dir, norm);  \n"

        "float reflect_dot = max( dot(view_dir, reflect_dir), 0.0 );  \n"
        "float spec = pow(reflect_dot, 32.0);    \n"
//        "vec4 specular_frag = specular * spec * light_color;   \n" 
        "vec4 specular_frag = vec4(0.0, 0.0, 0.0, 0.0);   \n" 

        "float diffuse = max(dot(norm, light_dir), 0.0);   \n"         
        "vec4 diffuse_frag = vec4(diffuse * light_color.r, diffuse * light_color.g, diffuse * light_color.b, 0.0);  \n"
        "gl_FragColor = ambient_frag + diffuse_frag + specular_frag;   \n"
//        "gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);   \n"
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

//GLint u_normal_map_location = -1;
GLint u_translate_location = -1;
GLuint vertex_texture_buffer;

float time_cycle = 0;
float delta_time = 0.0;
int diff_time = 0;
Uint32 last_time;
Uint32 last_frame_time;
Uint32 current_time;

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
SDL_GLContext gl_context;

SDL_Texture* light_texture;
//SDL_Texture* sprite_texture;

SDL_Surface* sprite_surface;
SDL_Surface* normal_surface;
SDL_Surface* light_surface;

int sprite_width;
int sprite_height;

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
    gl_context = SDL_GL_CreateContext( window );

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

        // The maxLength includes the NULL character
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

    glEnableVertexAttribArray(a_position_location);
    glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, STRIDE, vertex_texture_data);

    glEnableVertexAttribArray(a_texcoord_location);
    glVertexAttribPointer(a_texcoord_location, 2, GL_FLOAT, GL_FALSE, STRIDE, vertex_texture_data+8);


    glGenTextures( 1,
                   &circle_tex);

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);    
    glBindTexture(GL_TEXTURE_2D, circle_tex);
/*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 */
    sprite_surface = IMG_Load( "/sprites/circle.png" );

    if( !sprite_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }
    else { 
        printf("success creating surface\n");
    }
/*
    sprite_texture = SDL_CreateTextureFromSurface( renderer, sprite_surface );

    if( !sprite_texture ) {
        printf("failed to create texture: %s\n", IMG_GetError() );
        return 0;
    }

    SDL_QueryTexture( sprite_texture, 
                        NULL, NULL, 
                        &sprite_width, &sprite_height );
*/
    sprite_width = 128;
    sprite_height = 128;

    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                sprite_width,
                sprite_height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                sprite_surface );

    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface( sprite_surface );

    glGenTextures( 1,
                   &normal_tex);

    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);    
    glBindTexture(GL_TEXTURE_2D, normal_tex);
/*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 */
    normal_surface = IMG_Load( "/sprites/ball-normal.png" );

    if( !normal_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }

    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                sprite_width,
                sprite_height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                normal_surface );

    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface( normal_surface );

    //----------- LIGHT TEXTURE -----------------------
    glGenTextures( 1,
                   &light_tex);

    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_2D);    
    glBindTexture(GL_TEXTURE_2D, light_tex );

    light_surface = IMG_Load( "/sprites/light.png" );

    if( !light_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }

    light_texture = SDL_CreateTextureFromSurface( renderer, light_surface );

    if( !light_texture ) {
        printf("failed to create light texture: %s\n", IMG_GetError() );
        return 0;
    }

    SDL_QueryTexture( light_texture, 
                        NULL, NULL, 
                        &light_width, &light_height );


    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                light_width,
                light_height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                light_surface );

//====================
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface( light_surface );
    glUseProgram(program);
// -- END LIGHT TEXTURE ---

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    emscripten_set_main_loop(game_loop, 0, 0);
}

void game_loop() {
    input();

//    glMatrixMode( GL_MODELVIEW );
//    glLoadIdentity( );
    glActiveTexture(GL_TEXTURE0);

    glGetIntegerv(GL_CURRENT_PROGRAM,&sdl_program);

    glUseProgram(program);

    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
/*
    glUniform4f(u_translate_location, 
                ship_x, ship_y, 0, 0 );
*/

    glBindBuffer(GL_ARRAY_BUFFER, vertex_texture_buffer);
    glVertexAttribPointer(
        a_position_location,// attribute 0. No particular reason for 0, but must match the layout in the shader.
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        4 * sizeof(float),  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(a_texcoord_location);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_texture_buffer);
    glVertexAttribPointer(
        a_texcoord_location,// attribute 0. No particular reason for 0, but must match the layout in the shader.
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        4 * sizeof(float),  // stride
        (void*)(2 * sizeof(float)) // array buffer offset
    );

    glUniform1i(u_texture_location, 1);
    glUniform1i(u_normal_location, 1);
//    glUniform1f( u_light_pos_location, time_cycle );
    glUniform3f( u_light_pos_location, 
                (float)(light_x), (float)(600-light_y), (float)(light_z) );

    glDrawArrays(GL_TRIANGLES, 0, 6);
//    SDL_GL_SwapWindow(window);
    //SDL_GL_MakeCurrent(window, gl_context);
//        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
//        SDL_RenderClear( renderer );

//    glDisableVertexAttribArray(a_texcoord_location);    
    glUseProgram(sdl_program);
    draw_light_icon();
}

int mouse_down = 0;

void input() {
    SDL_Event event;

    if(SDL_PollEvent( &event ) ) 
	{
		switch (event.type) 
		{

            case SDL_MOUSEWHEEL: 
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
            case SDL_MOUSEMOTION:
			{
                if( mouse_down == 1 ) {
                    SDL_GetMouseState( &light_x, &light_y );
                }
            }
            case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
                        SDL_GetMouseState( &light_x, &light_y );
                        mouse_down = 1;
                        printf("YOU NEED TO MOVE THE LIGHT SOURCE\n");
                        //play_btn->MouseClick(light_x, light_y);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
            case SDL_MOUSEBUTTONUP:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
                        mouse_down = 0;
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
        }
    }
}

void draw_light_icon() { //SDL_Texture *tex, SDL_Rect *src, SDL_Rect *dest, float rad_rotation,
                         //   int alpha, int red, int green, int blue ) {
/*
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = light_width;
    src.h = light_height;
*/
    SDL_Rect dest;
    dest.x = light_x - light_width / 2;
    dest.y = light_y - light_height / 2;
    dest.w = light_width;
    dest.h = light_height;

    SDL_RenderCopy( renderer, light_texture, NULL, &dest );
}
