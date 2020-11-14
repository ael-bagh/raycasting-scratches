/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bagh <ael-bagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 17:14:08 by ael-bagh          #+#    #+#             */
/*   Updated: 2020/11/11 11:54:07 by ael-bagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <math.h>
#include <stdio.h>
#define WIDTH 2050
#define FOV

    int abc[5][10] ={
            {1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,1,0,1},
            {1,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1}
            };

typedef struct  s_vars {
        void    *mlx;
        void    *win;
}               t_vars;
t_vars    vars;

typedef struct s_player {
    int x;
    int y;
    int fov;
    int turnDirection;
    int walkDirection;
    double rotationAngle;
    int moveSpeed;
    double rotationSpeed;
} t_player;

t_player player;

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

typedef struct s_rays {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    int wasHitVertical;
    int isRayFacingUp;
    int isRayFacingDown;
    int isRayFacingLeft;
    int isRayFacingRight;
    int wallHitcontent;
}               t_rays[320];
t_data  image;
t_data  map;

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void player_init()
{
    player.fov = 60 * (M_PI / 180);
    player.x = 32 * 3 + 16;
    player.y = 32 * 3 + 16;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.rotationAngle = M_PI / 2;
    player.moveSpeed = 5;
    player.rotationSpeed = 2 * (M_PI / 180);
}

void    draw_cube(int l, int x, int y)
{
    int i;
    int j;

    i = x;
    j = y;
    while (j <l + y)
    {
        while (i < l + x)
        {
            my_mlx_pixel_put(&map, j, i, 0xFF0000);
            i++;
        }
        i = x;
        j++;
    }
}
// void cast_ray(float rayangle , int stripId)
// {
        
// }

// void cast_rays ()
// {
//     // float xp;
//     // float yp;
//     // int i;
//     // float c;
    
//     // c = 0;
//     // i = -1;
//     // while (++i < len)
//     // {
//     //     xp = (cos(player.rotationAngle) * i) + x;
//     //     yp = (sin(player.rotationAngle) * i) + y;
//     //     my_mlx_pixel_put(&map, xp, yp, 0xfabade);
//     // }
//     // mlx_put_image_to_window(vars.mlx, vars.win, map.img, x, y);
//     float rayAngle = player.rotationAngle - (player.fov / 2);
    
//     for (int stripId = 0; stripId < 320; stripId++)
//     {
//         castRay(rayAngle , stripId);
//         rayAngle += player.fov / 320;
//     }
// }

void draw_map(int row, int col,int m[row][col])
{
    int x;
    int y;

    x =0;
    y = 0;
    map.img = mlx_new_image(vars.mlx, 1000, 1000);
    map.addr = mlx_get_data_addr(map.img, &map.bits_per_pixel, &map.line_length,
                                &map.endian);
    for(int e=0; e<5; e++) 
    {
        for(int f=0;f<10;f++) 
        {
            if(m[e][f] == 1)
            {
                draw_cube(32, y, x);
            }    
            x = x + 32;   
        }
        x = 0;
        y = y + 32;
    }
}
double  normalangle(double rayangle)
{
    rayangle = fmod(rayangle, (2 * M_PI));
    if (rayangle < 0)
        rayangle = (2 * M_PI) + rayangle;
    return (rayangle);
}

int     hasWall(int x, int y)
{
    int ughx;
    int ughy;

    ughx = floor(x / 32);
    ughy = floor(y / 32);
    if(abc[ughy][ughx] != 0)
        return (1);
    else
    {
        return (0);
    }
    
}

void render()
{
    int moveStep;
    int newPlayerx;
    int newPlayery;
    
    moveStep = player.walkDirection * player.moveSpeed;
    player.rotationAngle = normalangle(player.rotationAngle);
    player.rotationAngle += player.turnDirection * player.rotationSpeed;
    newPlayerx = player.x + cos(player.rotationAngle) * moveStep;
    newPlayery = player.y + sin(player.rotationAngle) * moveStep;
    if (!hasWall(newPlayerx , newPlayery))
    {
        player.y = newPlayery;
        player.x = newPlayerx;    
    }
    draw_map(5,10,abc);
    //draw_rays(player.x, player.y, 100);
    mlx_put_image_to_window(vars.mlx, vars.win, map.img, 0, 0);
    mlx_destroy_image(vars.mlx, map.img); 
}

int keyPress(int keycode)
{
        if(keycode == 13)
            player.walkDirection = 1;
        if(keycode == 1)
            player.walkDirection = -1;    
        if(keycode == 0)
            player.turnDirection = -1;
        if(keycode == 2)
            player.turnDirection = 1;
        //printf("walkdDirection = %d\nturnDirection = %d\n", player.walkDirection, player.turnDirection);
        return (0);
}
int keyRelease(int keycode)
{
        if(keycode == 13)
            player.walkDirection = 0;
        if(keycode == 1)
            player.walkDirection = 0;
        if(keycode == 0)
            player.turnDirection = 0;
        if(keycode == 2)
            player.turnDirection = 0;
        //printf("walkdDirection = %d\nturnDirection = %d\n", player.walkDirection, player.turnDirection);
        return (0);
}

int move()
{
    mlx_hook(vars.win, 2, 0, keyPress, &vars);
    render();
    mlx_hook(vars.win, 3, 0, keyRelease, &vars);
    return (0);
}

int             main(void)
{
    player_init();
    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 1000, 1000, "Hello world!");
    //mlx_clear_window(vars.mlx,vars.win);
    //mlx_destroy_image(vars.mlx, img.img);
    //draw_map(5,10,abc);
    //draw_line(player.x, player.y, 100);
    mlx_loop_hook(vars.mlx,move,0);
    mlx_loop(vars.mlx);
}