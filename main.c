#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <UI/UI.h>

void compile_shaders ( UIButton_t *button, ui_mouse_state_t state )
{

    // Initialized data
    UIInstance_t  *instance                                = ui_get_active_instance(); // This is the current UI instance
    UITextInput_t *path_text_input                         = 0,
                  *vertex_file                             = 0,
                  *tessellation_control_file               = 0,
                  *tessellation_evaluation_file            = 0,
                  *geometry_file                           = 0,
                  *fragment_file                           = 0,
                  *compute_file                            = 0;

    char          *vulkan_sdk_path                         = getenv("VK_SDK_PATH"),    // Path to the Vulkan SDK
                  *path                                    = 0,
                  *vertex_file_name                        = 0,
                  *vertex_compile_command                  = 0,
                  *tessellation_control_file_name          = 0,
                  *tessellation_control_compile_command    = 0,
                  *tessellation_evaluation_file_name       = 0,
                  *tessellation_evaluation_compile_command = 0,
                  *geometry_file_name                      = 0,
                  *geometry_compile_command                = 0,
                  *fragment_file_name                      = 0,
                  *fragment_compile_command                = 0,
                  *compute_file_name                       = 0,
                  *compute_compile_command                 = 0;
    UIElement_t   *title_label                             = find_element(instance->active_window, "shader compiler error label");
    char          *output_path                             = calloc(1024, sizeof(u8));
    size_t         fail                                    = 0;

    FILE          *output = 0;
    title_label->element.label->y = 1000;

    // Get each text input 
    {

        // Initialized data
        UIElement_t* element = 0;

        // Get the path to the shader
        element                      = find_element(instance->active_window, "shader path");
        path_text_input              = element->element.text_input;

        // Get the vertex shader file name
        element                          = find_element(instance->active_window, "vertex shader path");
        vertex_file                      = element->element.text_input;
        if (strlen(vertex_file->text))
            vertex_file_name                 = vertex_file->text;

        // Get the tessellation control shader file name
        element                           = find_element(instance->active_window, "tessellation control shader path");
        tessellation_control_file         = element->element.text_input;
        if(strlen(tessellation_control_file->text))
            tessellation_control_file_name    = tessellation_control_file->text;

        // Get the tessellation evaluation file name
        element                           = find_element(instance->active_window, "tessellation evaluation shader path");
        tessellation_evaluation_file      = element->element.text_input;
        if (strlen(tessellation_evaluation_file->text))
            tessellation_evaluation_file_name = tessellation_evaluation_file->text;

        // Get the geometry shader file name
        element                           = find_element(instance->active_window, "geometry shader path");
        geometry_file                     = element->element.text_input;
        if (strlen(geometry_file->text))
            geometry_file_name            = geometry_file->text;

        // Get the fragment shader file name
        element                           = find_element(instance->active_window, "fragment shader path");
        fragment_file                     = element->element.text_input;
        if (strlen(fragment_file->text))
            fragment_file_name            = fragment_file->text;

        // Get the compute shader file name
        element                           = find_element(instance->active_window, "compute shader path");
        compute_file                      = element->element.text_input;
        if (strlen(compute_file->text))
            compute_file_name             = compute_file->text;
    }

    // Get the path to the shader
    path = path_text_input->text;

    // Error checking
    {
        if ( vulkan_sdk_path == (void *) 0 )
            goto no_vulkan_sdk;

        if ( ( vertex_file_name                  || 
               tessellation_control_file_name    ||
               tessellation_evaluation_file_name || 
               geometry_file_name                ||
               fragment_file_name                ||
               compute_file_name                 
             ) == 0 )
            goto no_shaders_specified;
    }

    // Generate glslc commands
    {

        // Generate a command to compile the vertex shader
        if (vertex_file_name) {

            // Allocate memory for the command
            vertex_compile_command = calloc(1024, sizeof(u8));

            // Generate the command
            sprintf(vertex_compile_command, "%s\\Bin\\glslc.exe -fshader-stage=vert \"%s/%s\" -o \"%s/vert.spv\"", vulkan_sdk_path, path, vertex_file_name, path);
        }

        // Generate a command to compile the tessellation control shader
        if (tessellation_control_file_name) {

            // Allocate memory for the command
            tessellation_control_compile_command = calloc(1024, sizeof(u8));

            // Generate the command
            sprintf(tessellation_control_compile_command, "%s\\Bin\\glslc.exe -fshader-stage=tesc \"%s/%s\" -o \"%s/tesc.spv\"", vulkan_sdk_path, path, tessellation_control_file_name, path);
        }

        // Generate a command to compile the tessellation evaluation shader
        if (tessellation_evaluation_file_name) {

            // Allocate memory for the command
            tessellation_evaluation_compile_command = calloc(1024, sizeof(u8));

            // Generate the command
            sprintf(tessellation_evaluation_compile_command, "%s\\Bin\\glslc.exe -fshader-stage=tese \"%s/%s\" -o \"%s/tese.spv\"", vulkan_sdk_path, path, tessellation_evaluation_file_name, path);
        }

        // Generate a command to compile the geometry shader
        if (geometry_file_name) {

            // Allocate memory for the command
            geometry_compile_command = calloc(1024, sizeof(u8));

            // Generate the command
            sprintf(geometry_compile_command, "%s\\Bin\\glslc.exe -fshader-stage=geom \"%s/%s\" -o \"%s/geom.spv\"", vulkan_sdk_path, path, geometry_file_name, path);
        }

        // Generate a command to compile the fragment shader
        if (fragment_file_name) {

            // Allocate memory for the command
            fragment_compile_command = calloc(1024, sizeof(u8));

            // Generate the command
            sprintf(fragment_compile_command, "%s\\Bin\\glslc.exe -fshader-stage=frag \"%s/%s\" -o \"%s/frag.spv\"", vulkan_sdk_path, path, fragment_file_name, path);
        }
        
        // Generate a command to compile the compute shader
        if (compute_file_name) {

            // Allocate memory for the command
            compute_compile_command = calloc(1024, sizeof(u8));

            // Generate the command
            sprintf(compute_compile_command, "%s\\Bin\\glslc.exe -fshader-stage=comp \"%s/%s\" -o \"%s/comp.spv\"", vulkan_sdk_path, path, compute_file_name, path);
        }
    }

    // Compile the shaders
    {

        // Compile the vertex shader
        if (vertex_compile_command)
        {
            
            // Run the glsl compiler
            if (system(vertex_compile_command))
                goto failed_to_compile_vertex;

            // Free the allocated memory
            free(vertex_compile_command);
        }

        compile_tesc:

        // Compile the tessellation control shader
        if (tessellation_control_compile_command)
        {

            // Run the glsl compiler
            if (system(tessellation_control_compile_command))
                goto failed_to_compile_tessellation_control;

            // Free the allocated memory
            free(tessellation_control_compile_command);
        }

        compile_tese:

        // Compile the tessellation evaluation shader
        if (tessellation_evaluation_compile_command)
        {

            // Run the glsl compiler
            if (system(tessellation_evaluation_compile_command))
                goto failed_to_compile_tessellation_evaluation;

            // Free the allocated memory
            free(tessellation_evaluation_compile_command);
        }

        compile_geom:

        // Compile the geometry shader
        if (geometry_compile_command)
        {

            // Run the glsl compiler
            if(system(geometry_compile_command))
                goto failed_to_compile_geometry;

            // Free the allocated memory
            free(geometry_compile_command);
        }

        compile_frag:

        // Compile the fragment shader
        if (fragment_compile_command)
        {

            // Run the glsl compiler
            if ( system(fragment_compile_command) )
                goto failed_to_compile_fragment;

            // Free the allocated memory
            free(fragment_compile_command);
        }

        compile_comp:

        // Compile the compute shader
        if (compute_compile_command)
        {

            // Run the glsl compiler
            if ( system(compute_compile_command) )
                goto failed_to_compile_compute;

            // Free the allocated memory
            free(compute_compile_command);
        }
    }
    
    close_file:
    if(fail)
        title_label->element.label->y = 342;

    return;

    // Error handling
    {

        // User errors
        {

            failed_to_compile_vertex:
                title_label->element.label->text = "ERROR: Failed to compile vertex shader!";
                fail = 1;
                goto compile_tesc;

            failed_to_compile_tessellation_control:
                title_label->element.label->text = "ERROR: Failed to compile tessellation control shader!";
                fail = 1;
                goto compile_tese;

            failed_to_compile_tessellation_evaluation:
                title_label->element.label->text = "ERROR: Failed to compile tessellation evaluation shader!";
                fail = 1;
                goto compile_geom;

            failed_to_compile_geometry:
                title_label->element.label->text = "ERROR: Failed to compile geometry shader!";
                fail = 1;
                goto compile_frag;

            failed_to_compile_fragment:
                title_label->element.label->text = "ERROR: Failed to compile fragment shader!";
                fail = 1;
                goto compile_comp;
            
            failed_to_compile_compute:
                title_label->element.label->text = "ERROR: Failed to compile compute shader!";
                fail = 1;
                goto close_file;

            no_vulkan_sdk:
                title_label->element.label->text = "ERROR: Please install the Vulkan SDK OR create a VK_SDK_PATH environment variable!";
                fail = 1;
                goto close_file;

            no_shaders_specified:
                title_label->element.label->text = "ERROR: Specify at least one shader path in the text inputs above!";
                fail = 1;
                goto close_file;
        }

    }
}

#define main WinMain

int main( int argc, const char *argv[] )
{

    // Initialized data
    UIInstance_t  *instance       = 0;
    UIWindow_t    *window         = 0;
    UIElement_t   *compile_button = 0;
    UITextInput_t *shader_path    = 0;
    char          *g10_directory  = getenv("G10_SOURCE_PATH");

    // Initialize the UI Library
    ui_init(&instance, "");

    // Load the window
    load_window(&window, "window.json");

    // Add the window to the instance
    ui_append_window(instance, window);

    // Find the compile button
    compile_button = find_element(window, "shader compile button");

    // Add a callback to the compile button
    add_click_callback_element(compile_button, compile_shaders);



    // Copy the G10 path
    {
        shader_path = find_element(window, "shader path");

        // Pre set the active textbox
        instance->active_window->last = shader_path;

        shader_path = ((UIElement_t*)shader_path)->element.text_input;
        
        shader_path->width = 8 * ( sprintf(shader_path->text, "%sG10\\shaders\\", (g10_directory) ? g10_directory : "") + 1);
    }

    // Start running
    instance->running = true;

    // UI looop
    while (instance->running)
    {

        // Get input
        ui_process_input(instance);

        // Draw windows
        ui_draw(instance);
    }

    // Exit
    ui_exit(instance);

    return 0;
}