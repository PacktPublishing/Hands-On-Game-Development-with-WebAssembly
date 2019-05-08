#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#include <SDL_opengl.h>
#include <GLES3/gl3.h>
#include <emscripten.h>

#ifdef DEBUG 
    void run_debug(const char* str) {
        EM_ASM (
            console.log(new Error().stack);
        );
        printf("%s\n", str);
    }

    #define DBG(str)  run_debug(str)
#else
    #define DBG(str) 
#endif

extern "C" {
    void inner_call_1() {
        DBG("check console log for stack trace");
    }
    void inner_call_2() {
        inner_call_1();
    }
    void inner_call_3() {
        inner_call_2();
    }
}

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600
#define FLOAT32_BYTE_SIZE 4
#define STRIDE FLOAT32_BYTE_SIZE*4
#define XY_OFFSET 0;
#define UV_OFFSET 8;

const GLchar* vertex_shader_code[] = {
      "precision mediump float; \n"

      "attribute vec4 a_position;   \n"
      "attribute vec2 a_texcoord;   \n"

      "uniform vec4 u_translate;    \n"

      "varying vec2 v_texcoord; \n"

      "void main() {    \n"
        "gl_Position = u_translate + a_position;    \n"
        "v_texcoord = a_texcoord;   \n"
      "}    \n"

};

const GLchar* fragment_shader_code[] = {
      "precision mediump float; \n"

      "varying vec2 v_texcoord; \n"

      "uniform sampler2D u_texture; \n"

      "void main() {    \n"
        "gl_FragColor = texture2D(u_texture, v_texcoord);   \n"
      "}    \n"
};


GLuint program = 0;
GLuint texture;

GLint a_texcoord_location = -1;
GLint a_position_location = -1;
GLint u_texture_location = -1;

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

SDL_Texture* sprite_texture;
SDL_Surface* sprite_surface;

int sprite_width;
int sprite_height;

float ship_x = 0.0;
float ship_y = 0.0;

void game_loop();

int main() {
    inner_call_3();
    
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
    u_translate_location = glGetUniformLocation(program, "u_translate");

    a_position_location = glGetAttribLocation(program, "a_position");
    a_texcoord_location = glGetAttribLocation(program, "a_texcoord");

    glGenBuffers(1, &vertex_texture_buffer);

    glBindBuffer( GL_ARRAY_BUFFER, vertex_texture_buffer );
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_data), 
                        vertex_texture_data, GL_STATIC_DRAW);

    sprite_surface = IMG_Load( "/sprites/spaceship.png" );

    if( !sprite_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }

    sprite_texture = SDL_CreateTextureFromSurface( renderer, sprite_surface );

    if( !sprite_texture ) {
        printf("failed to create texture: %s\n", IMG_GetError() );
        return 0;
    }

    SDL_QueryTexture( sprite_texture, 
                        NULL, NULL, 
                        &sprite_width, &sprite_height );

    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                sprite_width,
                sprite_height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                sprite_surface );

    SDL_FreeSurface( sprite_surface );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnableVertexAttribArray(a_position_location);
    glEnableVertexAttribArray(a_texcoord_location);

    glVertexAttribPointer(
        a_position_location,	// set up the a_position attribute
        2,                 		// how many attributes in the position
        GL_FLOAT,           	// data type of float
        GL_FALSE,           	// the data is not normalized
        4 * sizeof(float),  	// stride (how many array items until the next position)
        (void*)0            	// starting point for attribute
    );

    glVertexAttribPointer(
        a_texcoord_location,	// set up the a_texcoord attribute
        2,                  	// how many attributes in the texture coordinates
        GL_FLOAT,           	// data type of float
        GL_FALSE,           	// the data is not normalized
        4 * sizeof(float),  	// stride (how many array items until the next position)
        (void*)(2 * sizeof(float)) 	// starting point for attribute
    );

    emscripten_set_main_loop(game_loop, 0, 0);

}

void game_loop() {
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    ship_x += 0.002;
    ship_y += 0.001;

    if( ship_x >= 1.16 ) {
        ship_x = -1.16;
    }

    if( ship_y >= 1.21 ) {
        ship_y = -1.21;
    }
    
    glUniform4f(u_translate_location, 
                ship_x, ship_y, 0, 0 );

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
