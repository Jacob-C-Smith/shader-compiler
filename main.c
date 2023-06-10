#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <UI/UI.h>

enum shader_pipeline_e {
    invalid,
    graphics_standard,
    graphics_mesh,
    compute,
    ray
};

// Static data
static enum shader_pipeline_e type = 0;
int compile_shaders ( UIWindow_t *p_window );

int drop_operation ( UIWindow_t *p_window, char *path )
{

    // Initialized data
    UIInstance_t  *p_instance    = ui_get_active_instance();
    UITextInput_t *p_shader_path = find_element(p_window, "shader path");
    size_t         len           = ui_load_file(path, 0, false);
    char          *text          = calloc(len+1, sizeof(char));
    UIElement_t   *p_gi          = find_element(p_window, "graphics"),
                  *p_gt          = find_element(p_window, "graphics constructor title label"),
                  *p_vsp         = find_element(p_window, "vertex shader path"),
                  *p_vspl        = find_element(p_window, "vertex shader path label"),
                  *p_tcsp        = find_element(p_window, "tessellation control shader path"),
                  *p_tcspl       = find_element(p_window, "tessellation control shader path label"),
                  *p_tesp        = find_element(p_window, "tessellation evaluation shader path"),
                  *p_tespl       = find_element(p_window, "tessellation evaluation shader path label"),
                  *p_gsp         = find_element(p_window, "geometry shader path"),
                  *p_gspl        = find_element(p_window, "geometry shader path label"),
                  *p_tsp         = find_element(p_window, "task shader path"),
                  *p_tspl        = find_element(p_window, "task shader path label"),
                  *p_msp         = find_element(p_window, "mesh shader path"),
                  *p_mspl        = find_element(p_window, "mesh shader path label"),
                  *p_fsp         = find_element(p_window, "fragment shader path"),
                  *p_fspl        = find_element(p_window, "fragment shader path label"),
                  *p_ci          = find_element(p_window, "compute"),
                  *p_ct          = find_element(p_window, "compute constructor title label"),
                  *p_csp         = find_element(p_window, "compute shader path"),
                  *p_cspl        = find_element(p_window, "compute shader path label"),
                  *p_ri          = find_element(p_window, "ray"),
                  *p_rt          = find_element(p_window, "ray constructor title label"),
                  *p_rgsp        = find_element(p_window, "ray generation shader path"),
                  *p_rgspl       = find_element(p_window, "ray generation shader path label"),
                  *p_rahsp       = find_element(p_window, "ray any hit path"),
                  *p_rahspl      = find_element(p_window, "ray any hit shader path label"),
                  *p_rchsp       = find_element(p_window, "ray closest hit path"),
                  *p_rchspl      = find_element(p_window, "ray closest hit shader path label"),
                  *p_rmsp        = find_element(p_window, "ray miss path"),
                  *p_rmspl       = find_element(p_window, "ray miss shader path label"),
                  *p_risp        = find_element(p_window, "ray intersection path"),
                  *p_rispl       = find_element(p_window, "ray intersection shader path label"),
                  *p_rcsp        = find_element(p_window, "ray callable path"),
                  *p_rcspl       = find_element(p_window, "ray callable shader path label"),
                  *p_compb       = find_element(p_window, "shader compile button"),
                  *p_status      = find_element(p_window, "status label");

    bool iv = false;
    bool ff = false;
    bool pf = false;
    
    #ifdef _WIN64
        system("cls");
    #endif

    p_vspl->label->text[0] = ' ';
    p_tcspl->label->text[0] = ' ';
    p_tespl->label->text[0] = ' ';
    p_gspl->label->text[0] = ' ';
    p_tspl->label->text[0] = ' ';
    p_mspl->label->text[0] = ' ';
    p_fspl->label->text[0] = ' ';

    p_cspl->label->text[0] = ' ';

    p_rgspl->label->text[0] = ' ';
    p_rahspl->label->text[0] = ' ';
    p_rchspl->label->text[0] = ' ';
    p_rmspl->label->text[0] = ' ';
    p_rispl->label->text[0] = ' ';
    p_rcspl->label->text[0] = ' ';

    // Pre set the active textbox
    p_instance->active_window->last = p_shader_path;
    
    p_shader_path = ((UIElement_t*)p_shader_path)->text_input;
    #ifdef _WIN64
        p_shader_path->width = 8 * ( sprintf(p_shader_path->text, "%s", path) + 1);
    #else
        p_shader_path->width = 8 * ( sprintf(p_shader_path->text, "%s", path) + 1);
    #endif

    p_status->label->text[0]='\0';

    if ( type == graphics_standard )
    {
        size_t offset = 64;
        
        p_fsp->text_input->y += offset;
        p_fspl->label->y     += offset;
        p_status->label->y += offset;
    }
    else if ( type == graphics_mesh )
    {

        size_t offset = 128;

        p_msp->text_input->y += offset;
        p_mspl->label->y += offset;
        p_tsp->text_input->y += offset;
        p_tspl->label->y += offset;
        p_fsp->text_input->y += offset;
        p_fspl->label->y += offset;
        p_status->label->y += offset;
    }
    else if ( type == compute )
    {
        size_t offset = 256;
        
        p_ci->image->y += offset;
        p_ct->label->y += offset;
        p_csp->text_input->y += offset;
        p_cspl->label->y += offset;
        p_status->label->y += offset;
    }
    else if ( type == ray )
    {
        size_t offset = 322;

        p_ri->image->y += offset;
        p_rt->label->y += offset;

        p_rgsp->text_input->y += offset;
        p_rgspl->label->y += offset;
        p_rahsp->text_input->y += offset;
        p_rahspl->label->y += offset;
        p_rchsp->text_input->y += offset;
        p_rchspl->label->y += offset;
        p_rmsp->text_input->y += offset;
        p_rmspl->label->y += offset;
        p_risp->text_input->y += offset;
        p_rispl->label->y += offset;
        p_rcsp->text_input->y += offset;
        p_rcspl->label->y += offset;
        p_status->label->y += offset;
    }
    else if ( type == invalid )
    {
        p_status->label->y-=32;
    }

    // Load the file
    if ( ui_load_file(path, text, false) == 0 )
    {
        iv = true;
        ff = true;
    }

    // Parse the text into a JSON value
    JSONValue_t *p_value = 0,
                *p_vertex_shader_path                  = 0,
                *p_tessellation_control_shader_path    = 0,
                *p_tessellation_evaluation_shader_path = 0,
                *p_geometry_shader_path                = 0,
                *p_task_shader_path                    = 0,
                *p_mesh_shader_path                    = 0,
                *p_fragment_shader_path                = 0,
                *p_compute_shader_path                 = 0,
                *p_generation_shader_path              = 0,
                *p_any_hit_shader_path                 = 0,
                *p_closest_hit_shader_path             = 0,
                *p_miss_shader_path                    = 0,
                *p_intersection_shader_path            = 0,
                *p_callable_shader_path                = 0;

    if ( parse_json_value(text, 0, &p_value) == 0 )
    {
        iv = true;
        pf = true;
    }
    if ( p_value->type == JSONobject )
    {
        p_vertex_shader_path                  = dict_get(p_value->object, "vertex shader source path");
        p_tessellation_control_shader_path    = dict_get(p_value->object, "tessellation control shader source path");
        p_tessellation_evaluation_shader_path = dict_get(p_value->object, "tessellation evaluation shader source path");
        p_geometry_shader_path                = dict_get(p_value->object, "geometry shader source path");
        p_task_shader_path                    = dict_get(p_value->object, "task shader source path");
        p_mesh_shader_path                    = dict_get(p_value->object, "mesh shader source path");
        p_fragment_shader_path                = dict_get(p_value->object, "fragment shader source path");
        p_compute_shader_path                 = dict_get(p_value->object, "compute shader source path");        
        p_generation_shader_path              = dict_get(p_value->object, "ray generation shader source path");
        p_any_hit_shader_path                 = dict_get(p_value->object, "ray any hit shader source path");
        p_closest_hit_shader_path             = dict_get(p_value->object, "ray closest hit shader source path");
        p_miss_shader_path                    = dict_get(p_value->object, "ray miss shader source path");
        p_intersection_shader_path            = dict_get(p_value->object, "ray intersection shader source path");
        p_callable_shader_path                = dict_get(p_value->object, "ray callable shader source path");

        // Figure out the type of the shader
        if ( p_compute_shader_path )
            type = compute;
        else if (
            p_fragment_shader_path &&
            p_vertex_shader_path
        )
            type = graphics_standard;
        else if (
            p_fragment_shader_path &&
            p_mesh_shader_path
        )
            type = graphics_mesh;
        else if (
            p_generation_shader_path   ||
            p_any_hit_shader_path      ||
            p_closest_hit_shader_path  ||
            p_miss_shader_path         ||
            p_intersection_shader_path ||
            p_callable_shader_path 
        )
            type = ray;
        else
            type = invalid;

        {
            if ( p_vertex_shader_path ) p_vsp->text_input->width = 8 * ( sprintf(p_vsp->text_input->text, "%s", p_vertex_shader_path->string) + 1);
            else p_vsp->text_input->width = 8 * ( sprintf(p_vsp->text_input->text, "") + 1);

            if ( p_tessellation_control_shader_path ) p_tcsp->text_input->width = 8 * ( sprintf(p_tcsp->text_input->text, "%s", p_tessellation_control_shader_path->string) + 1);
            else p_tcsp->text_input->width = 8 * ( sprintf(p_tcsp->text_input->text, "") + 1);

            if ( p_tessellation_evaluation_shader_path ) p_tesp->text_input->width = 8 * ( sprintf(p_tesp->text_input->text, "%s", p_tessellation_evaluation_shader_path->string) + 1);
            else p_tesp->text_input->width = 8 * ( sprintf(p_tesp->text_input->text, "") + 1);

            if ( p_geometry_shader_path ) p_gsp->text_input->width = 8 * ( sprintf(p_gsp->text_input->text, "%s", p_geometry_shader_path->string) + 1);
            else p_gsp->text_input->width = 8 * ( sprintf(p_gsp->text_input->text, "") + 1);

            if ( p_task_shader_path ) p_tsp->text_input->width = 8 * ( sprintf(p_tsp->text_input->text, "%s", p_task_shader_path->string) + 1);
            else p_tsp->text_input->width = 8 * ( sprintf(p_tsp->text_input->text, "") + 1);

            if ( p_mesh_shader_path ) p_msp->text_input->width = 8 * ( sprintf(p_msp->text_input->text, "%s", p_mesh_shader_path->string) + 1);
            else p_msp->text_input->width = 8 * ( sprintf(p_msp->text_input->text, "") + 1);

            if ( p_fragment_shader_path ) p_fsp->text_input->width = 8 * ( sprintf(p_fsp->text_input->text, "%s", p_fragment_shader_path->string) + 1);
            else p_fsp->text_input->width = 8 * ( sprintf(p_fsp->text_input->text, "") + 1);

            if ( p_compute_shader_path ) p_csp->text_input->width = 8 * ( sprintf(p_csp->text_input->text, "%s", p_compute_shader_path->string) + 1);
            else p_csp->text_input->width = 8 * ( sprintf(p_csp->text_input->text, "") + 1);   

            if ( p_generation_shader_path ) p_rgsp->text_input->width = 8 * ( sprintf(p_rgsp->text_input->text, "%s", p_generation_shader_path->string) + 1);
            else p_rgsp->text_input->width = 8 * ( sprintf(p_rgsp->text_input->text, "") + 1);

            if ( p_any_hit_shader_path ) p_rahsp->text_input->width = 8 * ( sprintf(p_rahsp->text_input->text, "%s", p_any_hit_shader_path->string) + 1);
            else p_rahsp->text_input->width = 8 * ( sprintf(p_rahsp->text_input->text, "") + 1);

            if ( p_closest_hit_shader_path ) p_rchsp->text_input->width = 8 * ( sprintf(p_rchsp->text_input->text, "%s", p_closest_hit_shader_path->string) + 1);
            else p_rchsp->text_input->width = 8 * ( sprintf(p_rchsp->text_input->text, "") + 1);
            
            if ( p_miss_shader_path ) p_rmsp->text_input->width = 8 * ( sprintf(p_rmsp->text_input->text, "%s", p_miss_shader_path->string) + 1);
            else p_rmsp->text_input->width = 8 * ( sprintf(p_rmsp->text_input->text, "") + 1);
            
            if ( p_intersection_shader_path ) p_risp->text_input->width = 8 * ( sprintf(p_risp->text_input->text, "%s", p_intersection_shader_path->string) + 1);
            else p_risp->text_input->width = 8 * ( sprintf(p_risp->text_input->text, "") + 1);

            if ( p_callable_shader_path ) p_rcsp->text_input->width = 8 * ( sprintf(p_rcsp->text_input->text, "%s", p_callable_shader_path->string) + 1);
            else p_rcsp->text_input->width = 8 * ( sprintf(p_rcsp->text_input->text, "") + 1);
        }

        if ( type == graphics_standard )
        {
            p_msp->draw  = false;
            p_mspl->draw = false;
            p_tsp->draw  = false;
            p_tspl->draw = false;
            
            p_ci->draw = false;
            p_ct->draw = false;

            p_csp->draw  = false;
            p_cspl->draw = false;
            
            p_ri->draw = false;
            p_rt->draw = false;

            p_rgsp->draw   = false;
            p_rgspl->draw  = false;
            p_rahsp->draw  = false;
            p_rahspl->draw = false;
            p_rchsp->draw  = false;
            p_rchspl->draw = false;
            p_rmsp->draw   = false;
            p_rmspl->draw  = false;
            p_risp->draw   = false;
            p_rispl->draw  = false;
            p_rcsp->draw   = false;
            p_rcspl->draw  = false;

            size_t offset = 64;
            
            p_gi->draw = true;
            p_gt->draw = true;

            p_vsp->draw   = true;
            p_vspl->draw  = true;
            p_tcsp->draw  = true;
            p_tcspl->draw = true;
            p_tesp->draw  = true;
            p_tespl->draw = true;
            p_gsp->draw   = true;
            p_gspl->draw  = true;
            p_fsp->draw   = true;
            p_fspl->draw  = true;

            p_fsp->text_input->y -= offset;
            p_fspl->label->y     -= offset;
            p_status->label->y   -= offset;

            p_compb->button->y = 281;
            p_status->label->y = 276;

            p_window->height = 256+64-16;
            resize_window(p_window);
        }
        else if ( type == graphics_mesh )
        {

            p_vsp->draw    = false;
            p_vspl->draw   = false;
            p_tcsp->draw   = false;
            p_tcspl->draw  = false;
            p_tesp->draw   = false;
            p_tespl->draw  = false;
            p_gsp->draw    = false;
            p_gspl->draw   = false;

            p_ci->draw     = false;
            p_ct->draw     = false;

            p_csp->draw    = false;
            p_cspl->draw   = false;

            p_ri->draw     = false;
            p_rt->draw     = false;

            p_rgsp->draw   = false;
            p_rgspl->draw  = false;
            p_rahsp->draw  = false;
            p_rahspl->draw = false;
            p_rchsp->draw  = false;
            p_rchspl->draw = false;
            p_rmsp->draw   = false;
            p_rmspl->draw  = false;
            p_risp->draw   = false;
            p_rispl->draw  = false;
            p_rcsp->draw   = false;
            p_rcspl->draw  = false;

            size_t offset = 128;

            p_gi->draw     = true;
            p_gt->draw     = true;

            p_msp->draw  = true;
            p_mspl->draw = true;
            p_tsp->draw  = true;
            p_tspl->draw = true;
            p_fsp->draw  = true;
            p_fspl->draw = true;

            p_msp->text_input->y -= offset;
            p_mspl->label->y -= offset;
            p_tsp->text_input->y -= offset;
            p_tspl->label->y -= offset;
            p_fsp->text_input->y -= offset;
            p_fspl->label->y -= offset;
            p_status->label->y -= offset;
            

            p_compb->button->y = 281-64;
            p_status->label->y = 276-64;

            p_window->height = 256-16;
            resize_window(p_window);
        }
        else if ( type == compute )
        {
            p_gi->draw = false;
            p_gt->draw = false;

            p_vsp->draw    = false;
            p_vspl->draw   = false;
            p_tcsp->draw   = false;
            p_tcspl->draw  = false;
            p_tesp->draw   = false;
            p_tespl->draw  = false;
            p_gsp->draw    = false;
            p_gspl->draw   = false;
            p_msp->draw    = false;
            p_mspl->draw   = false;
            p_tspl->draw   = false;
            p_tsp->draw    = false;
            p_fsp->draw    = false;
            p_fspl->draw   = false;
            
            p_ri->draw = false;
            p_rt->draw = false;

            p_rgsp->draw   = false;
            p_rgspl->draw  = false;
            p_rahsp->draw  = false;
            p_rahspl->draw = false;
            p_rchsp->draw  = false;
            p_rchspl->draw = false;
            p_rmsp->draw   = false;
            p_rmspl->draw  = false;
            p_risp->draw   = false;
            p_rispl->draw  = false;
            p_rcsp->draw   = false;
            p_rcspl->draw  = false;

            size_t offset = 256;
            
            p_ci->draw   = true;
            p_ct->draw   = true;

            p_csp->draw  = true;
            p_cspl->draw = true;

            p_ci->image->y -= offset;
            p_ct->label->y -= offset;
            p_csp->text_input->y -= offset;
            p_cspl->label->y -= offset;
            p_status->label->y -= offset;

            p_compb->button->y = 281-64-64;
            p_status->label->y = 276-64-64;

            p_window->height = 256-16-64;
            resize_window(p_window);
        }
        else if ( type == ray )
        {
            p_gi->draw = false;
            p_gt->draw = false;

            p_vsp->draw    = false;
            p_vspl->draw   = false;
            p_tcsp->draw   = false;
            p_tcspl->draw  = false;
            p_tesp->draw   = false;
            p_tespl->draw  = false;
            p_gsp->draw    = false;
            p_gspl->draw   = false;
            p_msp->draw    = false;
            p_mspl->draw   = false;
            p_tspl->draw   = false;
            p_tsp->draw    = false;
            p_fsp->draw    = false;
            p_fspl->draw   = false;
            
            p_ci->draw = false;
            p_ct->draw = false;
            
            p_csp->draw = false;
            p_cspl->draw = false;
            
            size_t offset = 322;
            
            p_ri->draw = true;
            p_rt->draw = true;
            
            p_rgsp->draw   = true;
            p_rgspl->draw  = true;
            p_rahsp->draw  = true;
            p_rahspl->draw = true;
            p_rchsp->draw  = true;
            p_rchspl->draw = true;
            p_rmsp->draw   = true;
            p_rmspl->draw  = true;
            p_risp->draw   = true;
            p_rispl->draw  = true;
            p_rcsp->draw   = true;
            p_rcspl->draw  = true;

            p_ri->image->y -= offset;
            p_rt->label->y -= offset;

            p_rgsp->text_input->y -= offset;
            p_rgspl->label->y -= offset;
            p_rahsp->text_input->y -= offset;
            p_rahspl->label->y -= offset;
            p_rchsp->text_input->y -= offset;
            p_rchspl->label->y -= offset;
            p_rmsp->text_input->y -= offset;
            p_rmspl->label->y -= offset;
            p_risp->text_input->y -= offset;
            p_rispl->label->y -= offset;
            p_rcsp->text_input->y -= offset;
            p_rcspl->label->y -= offset;

            p_compb->button->y = 281+32;
            p_status->label->y = 281+26;

            p_window->height = 256+64+16;
            resize_window(p_window);
        }
        else if ( type == invalid )
        {
            inval:
            p_gi->draw = true,
            p_gt->draw = true,
            p_vsp->draw = true,
            p_vspl->draw = true,
            p_tcsp->draw = true,
            p_tcspl->draw = true,
            p_tesp->draw = true,
            p_tespl->draw = true,
            p_gsp->draw = true,
            p_gspl->draw = true,
            p_tsp->draw = true,
            p_tspl->draw = true,
            p_msp->draw = true,
            p_mspl->draw = true,
            p_fsp->draw = true,
            p_fspl->draw = true,
            p_ci->draw = true,
            p_ct->draw = true,
            p_csp->draw = true,
            p_cspl->draw = true,
            p_ri->draw = true,
            p_rt->draw = true,
            p_rgsp->draw = true,
            p_rgspl->draw = true,
            p_rahsp->draw = true,
            p_rahspl->draw = true,
            p_rchsp->draw = true,
            p_rchspl->draw = true,
            p_rmsp->draw = true,
            p_rmspl->draw = true,
            p_risp->draw = true,
            p_rispl->draw = true,
            p_rcsp->draw = true,
            p_rcspl->draw = true;
            p_compb->button->y = 641;
            p_window->height = 664;
            resize_window(p_window);
            p_status->label->c = 0x000000F0;
            if ( ff )
                sprintf(p_status->label->text, "\210 Failed to load shader from path!");
            else if ( pf )
                sprintf(p_status->label->text, "\210 Failed to parse file as JSON!");
            
            p_status->label->y = 634;

            return 0;
        }
        if ( iv == true )
        {
            iv = false;
            p_status->label->text[0]='\0';
            goto inval;
        }
    }
    else
        return 0; // TODO: goto wrong_value_type;

    if ( compile_shaders(p_window) == 1 )
    {
        p_status->label->c = 0x0000C000;
        sprintf(p_status->label->text, "\200 Shader compiled successfully!");
    }

    // Success
    return 1;
}

int compile_shaders ( UIWindow_t *p_window )
{

    // Initialized data
    UIInstance_t  *p_instance = ui_get_active_instance();
    UIElement_t   *p_path     = find_element(p_window, "shader path"),
                  *p_vsp      = find_element(p_window, "vertex shader path"),
                  *p_tcsp     = find_element(p_window, "tessellation control shader path"),
                  *p_tesp     = find_element(p_window, "tessellation evaluation shader path"),
                  *p_gsp      = find_element(p_window, "geometry shader path"),
                  *p_tsp      = find_element(p_window, "task shader path"),
                  *p_msp      = find_element(p_window, "mesh shader path"),
                  *p_fsp      = find_element(p_window, "fragment shader path"),
                  *p_csp      = find_element(p_window, "compute shader path"),
                  *p_rgsp     = find_element(p_window, "ray generation shader path"),
                  *p_rahsp    = find_element(p_window, "ray any hit path"),
                  *p_rchsp    = find_element(p_window, "ray closest hit path"),
                  *p_rmsp     = find_element(p_window, "ray miss path"),
                  *p_risp     = find_element(p_window, "ray intersection path"),
                  *p_rcsp     = find_element(p_window, "ray callable path"),
                  *p_compb    = find_element(p_window, "shader compile button"),
                  *p_status   = find_element(p_window, "status label"),
                  *p_vspl     = find_element(p_window, "vertex shader path label"),
                  *p_tcspl    = find_element(p_window, "tessellation control shader path label"),
                  *p_tespl    = find_element(p_window, "tessellation evaluation shader path label"),
                  *p_gspl     = find_element(p_window, "geometry shader path label"),
                  *p_tspl     = find_element(p_window, "task shader path label"),
                  *p_mspl     = find_element(p_window, "mesh shader path label"),
                  *p_fspl     = find_element(p_window, "fragment shader path label"),
                  *p_cspl     = find_element(p_window, "compute shader path label"),
                  *p_rgspl    = find_element(p_window, "ray generation shader path label"),
                  *p_rahspl   = find_element(p_window, "ray any hit shader path label"),
                  *p_rchspl   = find_element(p_window, "ray closest hit shader path label"),
                  *p_rmspl    = find_element(p_window, "ray miss shader path label"),
                  *p_rispl    = find_element(p_window, "ray intersection shader path label"),
                  *p_rcspl    = find_element(p_window, "ray callable shader path label");
    size_t         ret        = 1;
    
    system("cls");
    
    p_vspl->label->text[0] = ' ';
    p_tcspl->label->text[0] = ' ';
    p_tespl->label->text[0] = ' ';
    p_gspl->label->text[0] = ' ';
    p_tspl->label->text[0] = ' ';
    p_mspl->label->text[0] = ' ';
    p_fspl->label->text[0] = ' ';

    p_cspl->label->text[0] = ' ';

    p_rgspl->label->text[0] = ' ';
    p_rahspl->label->text[0] = ' ';
    p_rchspl->label->text[0] = ' ';
    p_rmspl->label->text[0] = ' ';
    p_rispl->label->text[0] = ' ';
    p_rcspl->label->text[0] = ' ';

    if ( type == graphics_standard )
    {
        if ( strlen(p_vsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_vsp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_vsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S vert \"%.*s\\%s\" -o \"%.*s\\%.*svert.spv\"", n, p_path->text_input->text, p_vsp->text_input->text, n, p_path->text_input->text, m-4, p_vsp->text_input->text);
            
            p_vspl->label->text[0] = ' ';
            
            if ( system(buf) )
            {
                p_status->label->c = 0x000000FF;
                p_vspl->label->text[0] = '!';
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto vert_done;
            }
            else
            {
                p_vspl->label->text[0] = '\200';
            }
        }
        vert_done:

        if ( strlen(p_tcsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_tcsp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_tcsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S tesc \"%.*s\\%s\" -o \"%.*s\\%.*stesc.spv\"", n, p_path->text_input->text, p_tcsp->text_input->text, n, p_path->text_input->text, m-4, p_tcsp->text_input->text);
            
            p_tcspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_tcspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto tesc_done;
            }
            else
            {
                p_tcspl->label->text[0] = '\200';
            }
        }
        tesc_done:

        if ( strlen(p_tesp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_tesp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_tesp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S tese \"%.*s\\%s\" -o \"%.*s\\%.*stese.spv\"", n, p_path->text_input->text, p_tesp->text_input->text, n, p_path->text_input->text, m-4, p_tesp->text_input->text);

            p_tespl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_tespl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto tese_done;
            }
            else
            {
                p_tespl->label->text[0] = '\200';
            }
        }
        tese_done:

        if ( strlen(p_gsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_gsp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_gsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S geom \"%.*s\\%s\" -o \"%.*s\\%.*sgeom.spv\"", n, p_path->text_input->text, p_gsp->text_input->text, n, p_path->text_input->text, m-4, p_gsp->text_input->text);

            p_gspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_gspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto geom_done;
            }
            else
            {
                p_gspl->label->text[0] = '\200';
            }
        }
        geom_done:

        if ( strlen(p_fsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_fsp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_fsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S frag \"%.*s\\%s\" -o \"%.*s\\%.*sfrag.spv\"", n, p_path->text_input->text, p_fsp->text_input->text, n, p_path->text_input->text, m-4, p_fsp->text_input->text);

            p_fspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_fspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto frag_done;
            }
            else
            {
                p_fspl->label->text[0] = '\200';
            }
        }
    }
    else if ( type == graphics_mesh )
    {
        if ( strlen(p_tsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_tsp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_tsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S task \"%.*s\\%s\" -o \"%.*s\\%.*stask.spv\"", n, p_path->text_input->text, p_tsp->text_input->text, n, p_path->text_input->text, m-4, p_tsp->text_input->text);

            p_tspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_tspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto task_done;
            }
            else
            {
                p_tspl->label->text[0] = '\200';
            }
        }
        task_done:

        if ( strlen(p_msp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_msp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_msp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S mesh \"%.*s\\%s\" -o \"%.*s\\%.*smesh.spv\"", n, p_path->text_input->text, p_msp->text_input->text, n, p_path->text_input->text, m-4, p_msp->text_input->text);

            p_mspl->label->text[0] = ' ';
            
            if ( system(buf) )
            {
                p_mspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto mesh_done;
            }
            else
            {
                p_mspl->label->text[0] = '\200';
            }
        }
        mesh_done:

        if ( strlen(p_fsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_fsp->text_input->text, '.');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_fsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S frag \"%.*s\\%s\" -o \"%.*s\\%.*sfrag.spv\"", n, p_path->text_input->text, p_fsp->text_input->text, n, p_path->text_input->text, m-4, p_fsp->text_input->text);

            p_fspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_fspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto frag_done;
            }
            else
            {
                p_fspl->label->text[0] = '\200';
            }
        }
    }
    else if ( type == compute )
    {
        if ( strlen(p_csp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_csp->text_input->text, '/');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_csp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S comp \"%.*s\\%s\" -o \"%.*s\\%.*s\\comp.spv\"", n, p_path->text_input->text, p_csp->text_input->text, n, p_path->text_input->text, m, p_csp->text_input->text);
            
            p_cspl->label->text[0] = ' ';
            
            if ( system(buf) )
            {
                p_cspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader!");
                ret=0;
                goto compute_done;
            }
            else
            {
                p_cspl->label->text[0] = '\200';
            }
        }
        compute_done:;

    }
    else if ( type == ray )
    {
        if ( strlen(p_rgsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_rgsp->text_input->text, '/');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_rgsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S rgen \"%.*s\\%s\" -o \"%.*s\\%.*s\\rgen.spv\"", n, p_path->text_input->text, p_rgsp->text_input->text, n, p_path->text_input->text, m, p_rgsp->text_input->text);

            p_rgspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_rgspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto ray_gen_done;
            }
            else
            {
                p_rgspl->label->text[0] = '\200';
            }
        }
        ray_gen_done:

        if ( strlen(p_rahsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_rahsp->text_input->text, '/');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_rahsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S rahit \"%.*s\\%s\" -o \"%.*s\\%.*s\\rahit.spv\"", n, p_path->text_input->text, p_rahsp->text_input->text, n, p_path->text_input->text, m, p_rahsp->text_input->text);

            p_rahspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_rahspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto ray_anyhit_done;
            }
            else
            {
                p_rahspl->label->text[0] = '\200';
            }
        }
        ray_anyhit_done:

        if ( strlen(p_rchsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_rchsp->text_input->text, '/');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_rchsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S rchit \"%.*s\\%s\" -o \"%.*s\\%.*s\\rchit.spv\"", n, p_path->text_input->text, p_rchsp->text_input->text, n, p_path->text_input->text, m, p_rchsp->text_input->text);

            p_rchspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_rchspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto ray_closest_hit_done;
            }
            else
            {
                p_rchspl->label->text[0] = '\200';
            }
        }
        ray_closest_hit_done:

        if ( strlen(p_rmsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_rmsp->text_input->text, '/');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_rmsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S rmiss \"%.*s\\%s\" -o \"%.*s\\%.*s\\rmiss.spv\"", n, p_path->text_input->text, p_rmsp->text_input->text, n, p_path->text_input->text, m, p_rmsp->text_input->text);
            
            p_rmspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_rmspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto ray_miss_done;
            }
            else
            {
                p_rmspl->label->text[0] = '\200';
            }
        }
        ray_miss_done:
        
        if ( strlen(p_risp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_risp->text_input->text, '/');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_risp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S rint \"%.*s\\%s\" -o \"%.*s\\%.*s\\rint.spv\"", n, p_path->text_input->text, p_risp->text_input->text, n, p_path->text_input->text, m, p_risp->text_input->text);

            p_rispl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_rispl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto ray_intersect_done;
            }
            else
            {
                p_rispl->label->text[0] = '\200';
            }
        }
        ray_intersect_done:
 
        if ( strlen(p_rcsp->text_input->text) )
        {
            char buf[4096] = { 0 };
            char *path = p_path->text_input->text;
            size_t z = strlen(path);
            char *n_path = strrchr(path, '\\');
            char *m_path = strrchr(p_rcsp->text_input->text, '/');
            size_t n = (size_t)n_path - (size_t)path;
            size_t m = (size_t)m_path - (size_t)p_rcsp->text_input->text;

            sprintf(buf, "glslangValidator --target-env vulkan1.3 -S rcall \"%.*s\\%s\" -o \"%.*s\\%.*s\\rcall.spv\"", n, p_path->text_input->text, p_rcsp->text_input->text, n, p_path->text_input->text, m, p_rcsp->text_input->text);
            p_rcspl->label->text[0] = ' ';

            if ( system(buf) )
            {
                p_rcspl->label->text[0] = '!';
                p_status->label->c = 0x000000FF;
                sprintf(p_status->label->text, "\210 Failed to compile shader(s)!");
                ret=0;
                goto ray_callable_done;
            }
            else
            {
                p_rispl->label->text[0] = '\200';
            }
        }
        ray_callable_done:;
 
    }
    else if ( type == invalid )
    {
        ret = -1;
    }

    frag_done:;
    return ret;
}

void compile_shaders_button (UIElement_t *p_element, ui_mouse_state_t mouse_state)
{
    UIWindow_t *p_window = (UIInstance_t*)(ui_get_active_instance())->active_window;
    size_t r = compile_shaders(p_window) == 1;

    if ( r == 1 )
    {
        UIElement_t *p_status = find_element(p_window, "status label");
        p_status->label->c = 0x0000C000;
        sprintf(p_status->label->text, "%c Shader compiled successfully!", '\200');
    }
    else if ( r == -1 )
    {
        UIElement_t *p_status = find_element(p_window, "status label");
        p_status->label->c = 0x0000C000;
        sprintf(p_status->label->text, "\210 Failed to load shader from path!");
    }
}

int main ( int argc, const char *argv[] )
{

    // Initialized data
    UIInstance_t  *p_instance       = 0;
    UIWindow_t    *p_window         = 0;
    UIElement_t   *p_compile_button = 0;
    UITextInput_t *p_shader_path    = 0;
    char          *g10_directory    = getenv("G10_SOURCE_PATH");

    system("title Shader Compiler Output");

    // Initialize the UI Library
    ui_init(&p_instance, "");

    // Load the window
    load_window(&p_window, "window.json");

    // Add the window to the instance
    ui_append_window(p_instance, p_window);

    // Find the compile button
    p_compile_button = find_element(p_window, "shader compile button");

    // Add a callback to the compile button
    add_click_callback_element(p_compile_button, compile_shaders_button);

    set_file_drop_operation(p_window, &drop_operation);

    // Copy the G10 path
    {
        p_shader_path = find_element(p_window, "shader path");
    
        // Pre set the active textbox
        p_instance->active_window->last = p_shader_path;
    
        p_shader_path = ((UIElement_t*)p_shader_path)->text_input;
        #ifdef _WIN64
        p_shader_path->width = 8 * ( sprintf(p_shader_path->text, "%sG10\\shaders\\", (g10_directory) ? g10_directory : "") + 1);
        #else
        p_shader_path->width = 8 * ( sprintf(p_shader_path->text, "%sG10/shaders/", (g10_directory) ? g10_directory : "") + 1);
        #endif
    }

    // Start running
    p_instance->running = true;

    // UI looop
    while (p_instance->running)
    {

        // Get input
        ui_process_input(p_instance);

        // Draw windows
        ui_draw(p_instance);
    }

    // Exit
    ui_exit(p_instance);
    
    system("pause");

    return 0;
}