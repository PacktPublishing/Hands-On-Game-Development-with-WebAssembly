#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_opengl.h>

#include <GLES2/gl2.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <emscripten.h>

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

      "varying vec2 v_texcoord; \n"

      "uniform vec4 u_translate;    \n"

      "void main() {    \n"
        "gl_Position = u_translate + a_position;    \n"
        "v_texcoord = a_texcoord;   \n"
      "}    \n"

};

const GLchar* fragment_shader_code[] = {
      "precision mediump float; \n"

      "varying vec2 v_texcoord; \n"

      "uniform float u_time;      \n"
      "uniform sampler2D u_texture; \n"
      "uniform sampler2D u_glow; \n"

      "void main() {    \n"
        "gl_FragColor = mix(texture2D(u_texture, v_texcoord), texture2D(u_glow, v_texcoord), texture2D(u_glow, v_texcoord).a);   \n"
      "}    \n"
};

GLuint program = 0;
GLuint texture, glow_tex;
GLuint normal_map;

GLint a_texcoord_location = -1;
GLint a_position_location = -1;
GLint u_texture_location = -1;
GLint u_glow_location = -1;

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
SDL_GLContext gl_context;

SDL_Texture* sprite_texture;
SDL_Surface* sprite_surface;
SDL_Surface* glow_surface;

//SDL_Rect source_rect = {.x = 0, .y = 0, .w = 128, .h = 32 };

int sprite_width;
int sprite_height;

float ship_x = 0.0;
float ship_y = 0.0;

void game_loop() {
    SDL_GL_MakeCurrent(window, gl_context);
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    glUseProgram(program);

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

    glEnableVertexAttribArray(a_position_location);
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

    glUniform1i(u_texture_location, 0);
    glUniform1i(u_glow_location, 1);
/*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, glow_tex);
*/
    glDrawArrays(GL_TRIANGLES, 0, 6);
    SDL_GL_SwapWindow(window);
}

int main() {
    printf("m1\n");

//    SDL_Rect dest = {.x = 160, .y = 100, .w = 0, .h = 0 };
    printf("m1.2\n");

    SDL_Init( SDL_INIT_VIDEO );
    printf("m1.4\n");

    SDL_CreateWindowAndRenderer( CANVAS_WIDTH, CANVAS_HEIGHT, 0, &window, &renderer );
    printf("m1.5\n");
    gl_context = SDL_GL_CreateContext( window );
    SDL_GL_MakeCurrent( window, gl_context );    

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    printf("m1.6\n");
    SDL_RenderClear( renderer );
    printf("m1.7\n");

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    printf("m2\n");
    glShaderSource(	vertex_shader,
                    1,
                    vertex_shader_code,
                    0);

    printf("m3\n");
    glCompileShader(vertex_shader);

    printf("m4\n");
    GLint compile_success = 0;
    printf("m5\n");
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_success);
    printf("m6\n");

    if(compile_success == GL_FALSE)
    {
        printf("failed to compile vertex shader\n");
        glDeleteShader(vertex_shader);
        return 0;
    }


    printf("m7\n");
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    printf("m8\n");
    glShaderSource(	fragment_shader,
                    1,
                    fragment_shader_code,
                    0);

    printf("m9\n");
    glCompileShader(fragment_shader);

    printf("m10\n");
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_success);

    printf("m11\n");
    if(compile_success == GL_FALSE)
    {
        printf("failed to compile fragment shader\n");
        glDeleteShader(fragment_shader);
        return 0;
    }

    printf("m12\n");
    program = glCreateProgram();

    printf("m13\n");
    glAttachShader(	program,
 	                vertex_shader);

    printf("m14\n");
    glAttachShader(	program,
 	                fragment_shader);

    printf("m15\n");
    glLinkProgram(program);

    printf("m16\n");
    GLint link_success = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &link_success);

    if (link_success == GL_FALSE)
    {
        printf("failed to link program\n");
        glDeleteProgram(program);
        return 0;
    }

    printf("m17\n");
    glUseProgram(program);


    printf("m18\n");

    u_texture_location = glGetUniformLocation(program, "u_texture");
    u_glow_location = glGetUniformLocation(program, "u_glow");
//    u_normal_map_location = glGetUniformLocation(program, "u_normal_map");
    u_translate_location = glGetUniformLocation(program, "u_translate");

    a_position_location = glGetAttribLocation(program, "a_position");
    a_texcoord_location = glGetAttribLocation(program, "a_texcoord");

    printf("m19.0\n");
    glGenBuffers(1, &vertex_texture_buffer);
    printf("m19.2\n");

    glActiveTexture(GL_TEXTURE0);
    glBindBuffer( GL_ARRAY_BUFFER, vertex_texture_buffer );
    printf("m19.3\n");
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_data), 
                 vertex_texture_data, GL_STATIC_DRAW);

    printf("m20\n");
    glEnableVertexAttribArray(a_position_location);
    glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, STRIDE, vertex_texture_data);

    glEnableVertexAttribArray(a_texcoord_location);
    glVertexAttribPointer(a_texcoord_location, 2, GL_FLOAT, GL_FALSE, STRIDE, vertex_texture_data+8);

    printf("m21.1\n");
    glGenTextures( 1,
                   &texture);

    printf("m21.2\n");
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);    
    glBindTexture(GL_TEXTURE_2D, texture);

    printf("m21.3\n");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    printf("m21.4\n");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
    printf("m22\n");
    sprite_surface = IMG_Load( "/sprites/spaceship.png" );

    if( !sprite_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }
    else { 
        printf("success creating surface\n");
    }

    printf("m23\n");
    sprite_texture = SDL_CreateTextureFromSurface( renderer, sprite_surface );

    if( !sprite_texture ) {
        printf("failed to create texture: %s\n", IMG_GetError() );
        return 0;
    }
    else { 
        printf("success creating asteroid texture\n");
    }

    printf("m24\n");
    SDL_QueryTexture( sprite_texture, 
                        NULL, NULL, 
                        &sprite_width, &sprite_height );

    printf("sprite_width=%d sprite_height=%d\n", 
            sprite_width, sprite_height );

//    glViewport(	0, 0, CANVAS_WIDTH, CANVAS_HEIGHT );

//    glBindTexture(GL_TEXTURE_2D, texture);

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
    //   glUniform1i(u_texture_location, 0);

//=========================================================================================

    glGenTextures( 1,
                   &glow_tex);

//    glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);    
    glBindTexture(GL_TEXTURE_2D, glow_tex);

    printf("m21.3\n");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    printf("m21.4\n");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
    printf("m22\n");
    glow_surface = IMG_Load( "/sprites/glow.png" );

    if( !glow_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }
    else { 
        printf("success creating surface\n");
    }

//    glViewport(	0, 0, CANVAS_WIDTH, CANVAS_HEIGHT );

//    glBindTexture(GL_TEXTURE_2D, glow_tex);

    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                sprite_width,
                sprite_height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                glow_surface );

    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface( glow_surface );

//    glUniform1i(u_glow_location, 1);

//=========================================================================================

    printf("m25\n");


//----------------------------------------------------------
/*
    printf("m26\n");
    glGenTextures( 1,
                   &normal_map);
 
    printf("m27\n");
    glBindTexture(GL_TEXTURE_2D, normal_map);

    printf("m28\n");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    printf("m29\n");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
    printf("m30\n");
    normal_surface = IMG_Load( "/sprites/normal.png" );

    if( !sprite_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }
    else { 
        printf("success creating surface\n");
    }
    printf("m31\n");

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_map);

    glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_RGBA,
                sprite_width,
                sprite_height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                normal_surface );

    printf("m32\n");
*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    printf("m26\n");
    emscripten_set_main_loop(game_loop, 0, 0);

}
