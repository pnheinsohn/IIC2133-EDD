#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "drawing.h"
#include <stdbool.h>

#define MAXSIZE 512.0

#define SUB_RADIUS 0.4

#define BUBBLES 3

double TILESIZE = 64.0;

static Color color_terrain = (Color){.R = 153.0/255.0, .G = 101.0/255.0, .B = 55.0/255.0};

static Color color_grass = (Color){.R = 31.0/255.0, .G = 187.0/255.0, .B = 89.0/255.0};

static Color color_sky = (Color){.R = 76.0/255.0, .G = 196.0/255.0, .B = 237.0/255.0};

static Color color_water = (Color){.R = 8.0/255.0, .G = 149.0/255.0, .B = 255.0/255.0};

static Color color_beak = (Color){.R = 255.0/255.0, .G = 186.0/255.0, .B = 0.0/255.0};

static Color color_spike = (Color){.R = 0.8, .G = 0.8, .B = 0.8};
static Color color_spike_box = (Color){.R = 0.5, .G = 0.5, .B = 0.5};
static Color color_spike_bolt = (Color){.R = 0.7, .G = 0.7, .B = 0.7};

static Color color_bird[3][2] =
{
  // {
  //   (Color){.R = 1, .G = 0.2, .B = 0.2},
  //   (Color){.R = 1, .G = 0.4, .B = 0.4}
  // },
  // {
  //   (Color){.R = 0.2, .G = 0.2, .B = 1},
  //   (Color){.R = 0.4, .G = 0.4, .B = 1}
  // },
  {
    (Color){.R = 255.0/255.0, .G = 0.0/255.0, .B = 0.0/255.0},
    (Color){.R = 255.0/255.0, .G = 76.0/255.0, .B = 48.0/255.0}
  },
  {
    (Color){.R = 0.0/255.0, .G = 76.0/255.0, .B = 255.0/255.0},
    (Color){.R = 0.0/255.0, .G = 117.0/255.0, .B = 220.0/255.0},
  },
  {
    (Color){.R = 24.0/255.0, .G = 209.0/255.0, .B = 31.0/255.0},
    (Color){.R = 71.0/255.0, .G = 219.0/255.0, .B = 72.0/255.0},
  }
};

/** El mutex de dibujo */
static pthread_mutex_t drawing_mutex;

/** Multiplica un color por un escalar, para aclararlo o oscurecerlo */
static Color color_scale(Color color, double k)
{
  return (Color){.R = color.R * k,.G = color.G * k, .B = color.B * k};
}

/** Setea el color RGB de cairo */
static void cairo_dip(cairo_t* cr, Color color)
{
  cairo_set_source_rgb(cr,color.R,color.G, color.B);
}

static void cairo_dip_alpha(cairo_t* cr, Color color, double alpha)
{
  cairo_set_source_rgba(cr,color.R,color.G, color.B, alpha);
}

static void cairo_circle(cairo_t* cr, double xc, double yc, double rad)
{
  cairo_arc(cr, xc, yc, rad, 0, 2*M_PI);
}

static void cairo_line(cairo_t* cr, double x1, double y1, double x2, double y2)
{
  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
}



static void cairo_bird_bubbles(cairo_t* cr, double x, double y, Direction next, int bubbles, int frac_length)
{
  cairo_new_sub_path (cr);

  double inc_l = TILESIZE / bubbles;

  if(next == DOWN)
  {
    cairo_move_to(cr, x, y + TILESIZE);


    for(int i = 0; i < bubbles; i++)
    {
      int a = abs(i - bubbles/2);
      double inc_c = TILESIZE / (frac_length + a);
      cairo_rel_curve_to(cr, 0, inc_c, inc_l, inc_c, inc_l, 0);
    }
  }
  else if(next == LEFT)
  {
    cairo_move_to(cr, x, y);

    for(int i = 0; i < bubbles; i++)
    {
      int a = abs(i - bubbles/2);
      double inc_c = TILESIZE / (frac_length + a);
      cairo_rel_curve_to(cr, -inc_c, 0, -inc_c, inc_l, 0, inc_l);
    }
  }
  else if(next == UP)
  {
    cairo_move_to(cr, x, y);

    for(int i = 0; i < bubbles; i++)
    {
      int a = abs(i - bubbles/2);
      double inc_c = TILESIZE / (frac_length + a);
      cairo_rel_curve_to(cr, 0, -inc_c, inc_l, -inc_c, inc_l, 0);
    }
  }
  else if(next == RIGHT)
  {
    cairo_move_to(cr, x + TILESIZE, y);
    for(int i = 0; i < bubbles; i++)
    {
      int a = abs(i - bubbles/2);
      double inc_c = TILESIZE / (frac_length + a);
      cairo_rel_curve_to(cr, inc_c, 0, inc_c, inc_l, 0, inc_l);
    }
  }
  cairo_close_path(cr);
}



static void cairo_bird_blob(cairo_t* cr, double x, double y, Direction next, Direction prev)
{
  // printf("NEXT = %u PREV %u\n", next, prev);

  double radius = SUB_RADIUS * TILESIZE;

  if(prev == next) radius /= 2;

  double degrees = M_PI / 180.0;

  cairo_new_sub_path (cr);
  if((next == DOWN || next == LEFT) && (prev == DOWN || prev == LEFT))
  {
    cairo_arc (cr, x + TILESIZE - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  }
  else
  {
    cairo_move_to(cr, x + TILESIZE, y);
  }

  if((next == LEFT || next == UP) && (prev == UP || prev == LEFT))
  {
    cairo_arc (cr, x + TILESIZE - radius, y + TILESIZE - radius, radius, 0 * degrees, 90 * degrees);
  }
  else
  {
    cairo_line_to(cr, x + TILESIZE, y + TILESIZE);
  }

  if((next == UP || next == RIGHT) && (prev == UP || prev == RIGHT))
  {
    cairo_arc (cr, x + radius, y + TILESIZE - radius, radius, 90 * degrees, 180 * degrees);
  }
  else
  {
    cairo_line_to(cr, x, y + TILESIZE);
  }

  if((next == RIGHT || next == DOWN) && (prev == DOWN || prev == RIGHT))
  {
    cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  }
  else
  {
    cairo_line_to(cr, x, y);
  }
  cairo_close_path (cr);
}

static void cairo_eye(cairo_t* cr, double xc, double yc, double R, double r, Direction dir)
{
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_circle(cr, xc, yc, R);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  if(dir == RIGHT) cairo_circle(cr, xc + R - r, yc, r);
  if(dir == LEFT) cairo_circle(cr, xc - R + r, yc, r);
  if(dir == UP) cairo_circle(cr, xc, yc - R + r, r);
  if(dir == DOWN) cairo_circle(cr, xc, yc + R - r, r);
  cairo_fill(cr);
}

static void cairo_draw_eyes(cairo_t* cr, double x, double y, Direction dir)
{
  double R = TILESIZE / 7.5;
  double r = TILESIZE / 20;

  if(dir == RIGHT)
  {
    double xc = x + TILESIZE / 2;
    double yc = y + TILESIZE / 4;

    cairo_eye(cr, xc, yc, R, r, dir);

    xc = x + 4 * TILESIZE / 5;

    cairo_eye(cr, xc, yc, R, r, dir);
  }
  else if(dir == LEFT)
  {
    double xc = x + TILESIZE / 2;
    double yc = y + TILESIZE / 4;

    cairo_eye(cr, xc, yc, R, r, dir);

    xc = x + TILESIZE / 5;

    cairo_eye(cr, xc, yc, R, r, dir);
  }
  else if(dir == UP)
  {
    double xc = x + TILESIZE / 4;
    double yc = y + TILESIZE / 2;

    cairo_eye(cr, xc, yc, R, r, dir);

    yc = y + TILESIZE / 5;

    cairo_eye(cr, xc, yc, R, r, dir);
  }
  else if(dir == DOWN)
  {
    double xc = x + 3 * TILESIZE / 4;
    double yc = y + TILESIZE / 2;

    cairo_eye(cr, xc, yc, R, r, dir);

    yc = y + 4 * TILESIZE / 5;

    cairo_eye(cr, xc, yc, R, r, dir);
  }
}

static void cairo_draw_spike(cairo_t* cr, int row, int col, int height, int width, int** matrix)
{
  double x = (col + 1) * TILESIZE;
  double y = (row + 1) * TILESIZE;

  double start_x, start_y;

  double SPIKE_POSITION = 3 * TILESIZE / 8;

  double SPIKE_LENGTH = TILESIZE / 3;
  double SPIKE_WIDTH = TILESIZE / 8;

  double SPIKE_BOX_SIDE = TILESIZE / 2;
  double SPIKE_BOX_TUBE = TILESIZE / 3;

  bool spike_down = false;
  bool spike_up = false;
  bool spike_left = false;
  bool spike_right = false;

  // TOP SIDE
  if(row == 0 || matrix[row - 1][col] == 0)
  {
    cairo_dip(cr, color_spike);
    start_x = x + SPIKE_POSITION - (SPIKE_WIDTH) / 2;
    start_y = y + SPIKE_LENGTH;

    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, +SPIKE_WIDTH / 2, -SPIKE_LENGTH);
    cairo_rel_line_to(cr, +SPIKE_WIDTH / 2, +SPIKE_LENGTH);
    cairo_close_path(cr);
    cairo_fill(cr);

    start_x = x + TILESIZE - SPIKE_POSITION + (SPIKE_WIDTH) / 2;
    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, -SPIKE_WIDTH / 2, -SPIKE_LENGTH);
    cairo_rel_line_to(cr, -SPIKE_WIDTH / 2, +SPIKE_LENGTH);
    cairo_close_path(cr);
    cairo_fill(cr);
  }
  else if(matrix[row - 1][col] == 2)
  {
    start_x = x + (TILESIZE - SPIKE_BOX_TUBE) / 2;
    start_y = y;

    cairo_dip(cr, color_spike_box);
    cairo_rectangle(cr, start_x, start_y, SPIKE_BOX_TUBE, TILESIZE / 2);
    cairo_fill(cr);

    spike_up = true;
  }

  // RIGHT SIDE
  if(col == width - 1 || matrix[row][col + 1] == 0)
  {
    cairo_dip(cr, color_spike);

    start_x = x + TILESIZE - SPIKE_LENGTH;
    start_y = y + SPIKE_POSITION - SPIKE_WIDTH / 2;

    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, +SPIKE_LENGTH, +SPIKE_WIDTH / 2);
    cairo_rel_line_to(cr, -SPIKE_LENGTH, +SPIKE_WIDTH / 2);
    cairo_close_path(cr);
    cairo_fill(cr);

    start_y = y + TILESIZE - SPIKE_POSITION + SPIKE_WIDTH / 2;

    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, +SPIKE_LENGTH, -SPIKE_WIDTH / 2);
    cairo_rel_line_to(cr, -SPIKE_LENGTH, -SPIKE_WIDTH / 2);
    cairo_close_path(cr);
    cairo_fill(cr);
  }
  else if (matrix[row][col + 1] == 2)
  {
    start_x = x + TILESIZE / 2;
    start_y = y + (TILESIZE - SPIKE_BOX_TUBE) / 2;

    cairo_dip(cr, color_spike_box);
    cairo_rectangle(cr, start_x, start_y, TILESIZE / 2, SPIKE_BOX_TUBE);
    cairo_fill(cr);

    spike_right = true;
  }

  // BOT SIDE
  if(row == height - 1 || matrix[row + 1][col] == 0)
  {
    cairo_dip(cr, color_spike);

    start_x = x + SPIKE_POSITION - (SPIKE_WIDTH) / 2;
    start_y = y + TILESIZE - SPIKE_LENGTH;


    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, +SPIKE_WIDTH / 2, +SPIKE_LENGTH);
    cairo_rel_line_to(cr, +SPIKE_WIDTH / 2, -SPIKE_LENGTH);
    cairo_close_path(cr);
    cairo_fill(cr);

    start_x = x + TILESIZE - SPIKE_POSITION + (SPIKE_WIDTH) / 2;
    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, -SPIKE_WIDTH / 2, +SPIKE_LENGTH);
    cairo_rel_line_to(cr, -SPIKE_WIDTH / 2, -SPIKE_LENGTH);
    cairo_close_path(cr);
    cairo_fill(cr);
  }
  else if(matrix[row + 1][col] == 2)
  {
    start_x = x + (TILESIZE - SPIKE_BOX_TUBE) / 2;
    start_y = y + TILESIZE / 2;

    cairo_dip(cr, color_spike_box);
    cairo_rectangle(cr, start_x, start_y, SPIKE_BOX_TUBE, TILESIZE / 2);
    cairo_fill(cr);

    spike_down = true;
  }

  // LEFT SIDE
  if(col == 0 || matrix[row][col - 1] == 0)
  {
    cairo_dip(cr, color_spike);

    start_x = x + SPIKE_LENGTH;
    start_y = y + SPIKE_POSITION - SPIKE_WIDTH / 2;

    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, -SPIKE_LENGTH, +SPIKE_WIDTH / 2);
    cairo_rel_line_to(cr, +SPIKE_LENGTH, +SPIKE_WIDTH / 2);
    cairo_close_path(cr);
    cairo_fill(cr);

    start_y = y + TILESIZE - SPIKE_POSITION + SPIKE_WIDTH / 2;

    cairo_new_sub_path(cr);
    cairo_move_to(cr, start_x, start_y);
    cairo_rel_line_to(cr, -SPIKE_LENGTH, -SPIKE_WIDTH / 2);
    cairo_rel_line_to(cr, +SPIKE_LENGTH, -SPIKE_WIDTH / 2);
    cairo_close_path(cr);
    cairo_fill(cr);
  }
  else if (matrix[row][col - 1] == 2)
  {
    start_x = x;
    start_y = y + (TILESIZE - SPIKE_BOX_TUBE) / 2;

    cairo_dip(cr, color_spike_box);
    cairo_rectangle(cr, start_x, start_y, TILESIZE / 2, SPIKE_BOX_TUBE);
    cairo_fill(cr);

    spike_left = true;
  }

  bool not_spike_box = (spike_up && spike_down) || (spike_left && spike_right);






  if(!not_spike_box)
  {

    cairo_dip(cr, color_spike_box);

    bool grounded = false;

    if(!grounded && row < height - 1 && matrix[row+1][col] == 1)
    {
      start_x = x + (TILESIZE - SPIKE_BOX_TUBE) / 2;
      start_y = y + TILESIZE / 2;

      cairo_rectangle(cr, start_x, start_y, SPIKE_BOX_TUBE, TILESIZE / 2);
      cairo_fill(cr);
      grounded = true;
    }
    if(!grounded && col < width - 1 && matrix[row][col+1] == 1)
    {
      start_x = x + TILESIZE / 2;
      start_y = y + (TILESIZE - SPIKE_BOX_TUBE) / 2;

      cairo_rectangle(cr, start_x, start_y, TILESIZE / 2, SPIKE_BOX_TUBE);
      cairo_fill(cr);
      grounded = true;
    }
    if(!grounded && row > 0 && matrix[row-1][col] == 1)
    {
      start_x = x + (TILESIZE - SPIKE_BOX_TUBE) / 2;
      start_y = y;

      cairo_rectangle(cr, start_x, start_y, SPIKE_BOX_TUBE, TILESIZE / 2);
      cairo_fill(cr);
      grounded = true;

    }
    if(!grounded && col > 0 && matrix[row][col-1] == 1)
    {
      start_x = x;
      start_y = y + (TILESIZE - SPIKE_BOX_TUBE) / 2;

      cairo_rectangle(cr, start_x, start_y, TILESIZE / 2, SPIKE_BOX_TUBE);
      cairo_fill(cr);
      grounded = true;
    }


    cairo_rectangle(cr, x + (TILESIZE - SPIKE_BOX_SIDE) / 2, y + (TILESIZE - SPIKE_BOX_SIDE) / 2, SPIKE_BOX_SIDE, SPIKE_BOX_SIDE);
    cairo_fill(cr);

    cairo_dip(cr, color_spike_bolt);

    cairo_circle(cr, x + TILESIZE/2, y + TILESIZE/2, SPIKE_BOX_SIDE / 3);
    cairo_fill(cr);
    cairo_dip(cr, color_spike_box);

    double dx = (TILESIZE - SPIKE_BOX_SIDE) / 2 + TILESIZE*0.1;
    double dy =  TILESIZE - (TILESIZE - SPIKE_BOX_SIDE) / 2 - TILESIZE*0.1;


    cairo_line(cr, x + dx, y + dy, x + dy, y + dx);
    cairo_set_line_width(cr, 3);
    cairo_stroke(cr);
  }


}

static void cairo_draw_beak(cairo_t* cr, double x, double y, Direction dir)
{
  if(dir == RIGHT) x -= TILESIZE * 0.1;
  if(dir == LEFT) x += TILESIZE * 0.1;
  if(dir == UP) y += TILESIZE * 0.1;
  if(dir == DOWN) y -= TILESIZE * 0.1;


  double BEAK_LENGTH = 2 * TILESIZE / 4;
  double BEAK_DEPTH_OUT = 0.5;
  double BEAK_DEPTH_IN = 0.25;
  double BEAK_WIDTH_BASE = TILESIZE / 3;
  double BEAK_WIDTH_EDGE = TILESIZE / 5;

  if(dir == UP)
  {
    double x_start = x + (TILESIZE - BEAK_WIDTH_BASE) / 2;
    double y_start = y + BEAK_LENGTH / 2;

    cairo_dip(cr, color_beak);
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2, -BEAK_LENGTH);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, -BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2, BEAK_LENGTH);
    cairo_close_path(cr);

    cairo_fill(cr);

    x_start = x + (TILESIZE - BEAK_WIDTH_EDGE) / 2;
    y_start = y - BEAK_LENGTH / 2;

    cairo_dip(cr, color_scale(color_beak, 0.5));
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, -BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, -(BEAK_WIDTH_EDGE) / 2, BEAK_LENGTH * (BEAK_DEPTH_OUT + BEAK_DEPTH_IN));
    cairo_close_path(cr);

    cairo_fill(cr);
  }
  else if(dir == DOWN)
  {
    double x_start = x + (TILESIZE - BEAK_WIDTH_BASE) / 2;
    double y_start = y + TILESIZE - BEAK_LENGTH / 2;

    cairo_dip(cr, color_beak);
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2, BEAK_LENGTH);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, -BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2, -BEAK_LENGTH);
    cairo_close_path(cr);

    cairo_fill(cr);

    x_start = x + (TILESIZE - BEAK_WIDTH_EDGE) / 2;
    y_start = y  + TILESIZE + BEAK_LENGTH / 2;

    cairo_dip(cr, color_scale(color_beak, 0.5));
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, -BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, (BEAK_WIDTH_EDGE) / 2, BEAK_LENGTH * BEAK_DEPTH_OUT);
    cairo_rel_line_to(cr, -(BEAK_WIDTH_EDGE) / 2, -BEAK_LENGTH * (BEAK_DEPTH_OUT + BEAK_DEPTH_IN));
    cairo_close_path(cr);

    cairo_fill(cr);
  }
  else if(dir == RIGHT)
  {
    double x_start = x + TILESIZE - BEAK_LENGTH / 2;
    double y_start = y + (TILESIZE - BEAK_WIDTH_BASE) / 2;

    cairo_dip(cr, color_beak);
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, BEAK_LENGTH, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2);
    cairo_rel_line_to(cr, -BEAK_LENGTH * BEAK_DEPTH_OUT, (BEAK_WIDTH_EDGE) / 2);
    cairo_rel_line_to(cr, BEAK_LENGTH * BEAK_DEPTH_OUT, (BEAK_WIDTH_EDGE) / 2);
    cairo_rel_line_to(cr, -BEAK_LENGTH, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2);

    cairo_close_path(cr);

    cairo_fill(cr);

    x_start = x + TILESIZE + BEAK_LENGTH / 2;
    y_start = y + (TILESIZE - BEAK_WIDTH_EDGE) / 2;

    cairo_dip(cr, color_scale(color_beak, 0.5));
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, -BEAK_LENGTH * BEAK_DEPTH_OUT, BEAK_WIDTH_EDGE / 2);
    cairo_rel_line_to(cr, BEAK_LENGTH * BEAK_DEPTH_OUT, BEAK_WIDTH_EDGE / 2);
    cairo_rel_line_to(cr, -BEAK_LENGTH * (BEAK_DEPTH_OUT + BEAK_DEPTH_IN), -BEAK_WIDTH_EDGE / 2);
    cairo_close_path(cr);

    cairo_fill(cr);
  }
  else if(dir == LEFT)
  {
    double x_start = x + BEAK_LENGTH / 2;
    double y_start = y + (TILESIZE - BEAK_WIDTH_BASE) / 2;

    cairo_dip(cr, color_beak);
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, -BEAK_LENGTH, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2);
    cairo_rel_line_to(cr, BEAK_LENGTH * BEAK_DEPTH_OUT, (BEAK_WIDTH_EDGE) / 2);
    cairo_rel_line_to(cr, -BEAK_LENGTH * BEAK_DEPTH_OUT, (BEAK_WIDTH_EDGE) / 2);
    cairo_rel_line_to(cr, BEAK_LENGTH, (BEAK_WIDTH_BASE - BEAK_WIDTH_EDGE) / 2);

    cairo_close_path(cr);

    cairo_fill(cr);

    x_start = x - BEAK_LENGTH / 2;
    y_start = y + (TILESIZE - BEAK_WIDTH_EDGE) / 2;

    cairo_dip(cr, color_scale(color_beak, 0.5));
    cairo_new_sub_path(cr);

    cairo_move_to(cr, x_start, y_start);
    cairo_rel_line_to(cr, BEAK_LENGTH * BEAK_DEPTH_OUT, BEAK_WIDTH_EDGE / 2);
    cairo_rel_line_to(cr, -BEAK_LENGTH * BEAK_DEPTH_OUT, BEAK_WIDTH_EDGE / 2);
    cairo_rel_line_to(cr, BEAK_LENGTH * (BEAK_DEPTH_OUT + BEAK_DEPTH_IN), -BEAK_WIDTH_EDGE / 2);
    cairo_close_path(cr);

    cairo_fill(cr);
  }
}

static void cairo_draw_head(cairo_t* cr, double x, double y, Direction dir)
{
  cairo_draw_eyes(cr, x, y, dir);
  cairo_draw_beak(cr, x, y, dir);
}

static void reset_layer(cairo_surface_t* layer)
{
  cairo_t* cr = cairo_create(layer);

  cairo_dip_alpha(cr, color_init(0, 0, 0), 0);

  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);

  cairo_paint(cr);

  cairo_destroy(cr);
}

void drawing_reset_birds (Content* cont)
{
  reset_layer(cont -> bird_layer);
  reset_layer(cont -> face_layer);
}

/** Dibuja un pájaro en la ventana */
void drawing_draw_bird (Content* cont, int row, int col, int size, Direction* array)
{
  drawing_reset_birds(cont);

  int color = cont -> bird_color;
  /* Dibuja el cuerpo del pajaro */
  cairo_t* cr = cairo_create(cont -> bird_layer);

  cairo_dip(cr, color_bird[color][0]);

  int curr_row = row;
  int curr_col = col;

  double x = TILESIZE * (curr_col + 1);
  double y = TILESIZE * (curr_row + 1);


  /* Dibuja la cara del pajaro */
  cairo_t* fcr = cairo_create(cont -> face_layer);
  cairo_draw_head(fcr, x, y, OPPOSITE(array[0]));
  cairo_destroy(fcr);


  cairo_bird_blob(cr, x, y, array[0], array[0]);
  cairo_fill(cr);

  for(int i = 0; i < size - 1; i++)
  {
    Direction dir = array[i];
    switch(dir)
    {
      case UP: curr_row--; break;
      case LEFT: curr_col--; break;
      case RIGHT: curr_col++; break;
      case DOWN: curr_row++; break;
      case HEAD:
      {
        fprintf(stderr, "Dirección invalida al dibujar: HEAD");
        abort();
        break;
      }
    }

    x = TILESIZE * (curr_col + 1);
    y = TILESIZE * (curr_row + 1);

    cairo_dip(cr, color_bird[color][(1 + i) % 2]);


    Direction next = i == size - 2 ? array[i] : array[i+1];
    Direction prev = OPPOSITE(array[i]);

    cairo_bird_blob(cr, x, y, next, prev);
    cairo_fill(cr);
  }

  cairo_dip(cr, color_bird[color][0]);

  curr_row = row;
  curr_col = col;
  x = TILESIZE * (curr_col + 1);
  y = TILESIZE * (curr_row + 1);

  cairo_bird_bubbles(cr, x, y, array[0], 3, 5);
  cairo_fill(cr);

  for(int i = 0; i < size - 1; i++)
  {
    Direction dir = array[i];
    switch(dir)
    {
      case UP: curr_row--; break;
      case LEFT: curr_col--; break;
      case RIGHT: curr_col++; break;
      case DOWN: curr_row++; break;
      case HEAD:
      {
        fprintf(stderr, "Dirección invalida al dibujar: HEAD");
        abort();
        break;
      }
    }

    x = TILESIZE * (curr_col + 1);
    y = TILESIZE * (curr_row + 1);

    cairo_dip(cr, color_bird[color][(1 + i) % 2]);


    Direction next = i == size - 2 ? array[i] : array[i+1];

    cairo_bird_bubbles(cr, x, y, next, 3, 5);
    cairo_fill(cr);
  }

  cairo_destroy(cr);
}

bool drawing_draw(cairo_t* restrict cr, Content* restrict cont)
{
  pthread_mutex_lock(&drawing_mutex);

  cairo_set_source_surface(cr, cont -> background_layer, 0, 0);

  cairo_paint(cr);

  cairo_set_source_surface(cr, cont -> portal_layer, (cont -> portal_col + 1) * TILESIZE, (cont -> portal_row + 1) * TILESIZE);

  cairo_paint(cr);

  cairo_set_source_surface(cr, cont -> bird_layer, 0, 0);

  cairo_paint(cr);

  cairo_set_source_surface(cr, cont -> terrain_layer, 0, 0);

  cairo_paint(cr);

  cairo_set_source_surface(cr, cont -> face_layer, 0, 0);

  cairo_paint(cr);

  pthread_mutex_unlock(&drawing_mutex);

	return true;
}


/** Establece una matriz de terreno como la capa de fondo de la ventana */
static void drawing_set_terrain  (Content* cont, FILE* f, int height, int width)
{
  cairo_t* bg_cr = cairo_create(cont -> background_layer);

  cairo_dip(bg_cr, color_scale(color_sky, 0.9));

  cairo_paint(bg_cr);

  cairo_t* t_cr = cairo_create(cont -> terrain_layer);

  int** matrix = calloc(height, sizeof(int*));

  for(int row = 0; row < height; row++)
  {
    matrix[row] = calloc(width, sizeof(int));
    for(int col = 0; col < width; col++)
    {

      fscanf(f, "%d", &matrix[row][col]);
    }
  }

  for(int row = 0; row < height; row++)
  {
    for(int col = 0; col < width; col++)
    {
      double x = TILESIZE * (col + 1);
      double y = TILESIZE * (row + 1);
      if(matrix[row][col] == 1)
      {
        cairo_dip(t_cr, color_terrain);
        cairo_rectangle(t_cr, x, y, TILESIZE, TILESIZE);
        cairo_fill(t_cr);
        if(row == 0 || matrix[row - 1][col] != 1)
        {
          cairo_dip(t_cr, color_grass);
          cairo_bird_bubbles(t_cr, x, y - TILESIZE * 0.75, DOWN, 4, 6);
          cairo_fill(t_cr);
          cairo_rectangle(t_cr, x, y, TILESIZE, TILESIZE * 0.25);
          cairo_fill(t_cr);
        }
      }
      else
      {
        cairo_dip(bg_cr, color_sky);
        cairo_rectangle(bg_cr, x, y, TILESIZE, TILESIZE);
        cairo_fill(bg_cr);
      }
      if(matrix[row][col] == 2)
      {
        cairo_draw_spike(t_cr, row, col, height, width, matrix);
      }
    }
  }

  // Dibujamos el "agua"

  cairo_dip(t_cr, color_scale(color_water, 0.8));
  cairo_rectangle(t_cr, 0 , (height + 1) * TILESIZE, (width + 2)*TILESIZE, TILESIZE);
  cairo_fill(t_cr);

  cairo_dip(t_cr, color_scale(color_water, 0.5));
  cairo_rectangle(t_cr, 0 , (height + 1.1) * TILESIZE, (width + 2)*TILESIZE, TILESIZE);
  cairo_fill(t_cr);

  cairo_dip(t_cr, color_scale(color_water, 0.3));
  cairo_rectangle(t_cr, 0 , (height + 1.3) * TILESIZE, (width + 2)*TILESIZE, TILESIZE);
  cairo_fill(t_cr);

  cairo_surface_t* portal_image = cairo_image_surface_create_from_png("assets/rainbow.png");


  fscanf(f, "%d %d", &cont -> portal_row, &cont -> portal_col);

  cairo_t* p_cr = cairo_create(cont -> portal_layer);

  double w = cairo_image_surface_get_width (portal_image);
  double h = cairo_image_surface_get_height (portal_image);

  cairo_scale (p_cr, TILESIZE / w, TILESIZE / h);

  cairo_set_source_surface(p_cr, portal_image, 0, 0);

  cairo_paint(p_cr);



  cairo_surface_destroy(portal_image);

  cairo_destroy(t_cr);
  cairo_destroy(p_cr);
  cairo_destroy(bg_cr);

  for(int row = 0; row < height; row++)
  {
    free(matrix[row]);
  }
  free(matrix);
}

/** Genera los contenedores para las dos imagenes superpuestas */
Content* drawing_init(char* filename)
{
  Content* cont = malloc(sizeof(Content));

  FILE* f = fopen(filename, "r");

  int height, width;
  fscanf(f, "%d %d", &height, &width);

  if(height > width)
  {
    TILESIZE = fminf(TILESIZE, MAXSIZE / ((double)height + 2));
  }
  else
  {
    TILESIZE = fminf(TILESIZE, MAXSIZE / ((double)width + 2));
  }

  TILESIZE = round(TILESIZE);

  cont -> height = (height + 2) * TILESIZE;
  cont -> width = (width + 2) * TILESIZE;

  /* Inicializa las imagenes vectoriales */

  // El formato de imagen: R G B de 8 bits cada uno + Alpha
  cairo_format_t format = CAIRO_FORMAT_ARGB32;
  cont -> bird_layer = cairo_image_surface_create(format, cont -> width, cont -> height);
  cont -> face_layer = cairo_image_surface_create(format, cont -> width, cont -> height);
  cont -> portal_layer = cairo_image_surface_create(format, cont -> width, cont -> height);
  cont -> terrain_layer = cairo_image_surface_create(format, cont -> width, cont -> height);
  cont -> background_layer = cairo_image_surface_create(format, cont -> width, cont -> height);

  drawing_set_terrain(cont, f, height, width);

  fclose(f);

  cont -> bird_color = (width ^ height) % 3;

	pthread_mutex_init(&drawing_mutex, NULL);

  return cont;
}



/** Geenera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename)
{
	double width = cont -> width;
	double height = cont -> height;

	/* Imprimimos las imagenes del tablero */
	cairo_surface_t* surface;
	cairo_t *cr;

  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB30, width, height);

	// surface = cairo_pdf_surface_create (filename, width, height);
	cr = cairo_create(surface);

	/* Reseteamos los parámetros para generar correctamente la imagen */
	Content aux = *cont;

	/* Dibuja el estado actual */
	drawing_draw(cr, &aux);

  cairo_surface_write_to_png(surface, filename);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

/** Libera los recursos asociados a las herramientas de dibujo */
void drawing_free(Content* cont)
{
  cairo_surface_destroy(cont -> bird_layer);
  cairo_surface_destroy(cont -> background_layer);
  cairo_surface_destroy(cont -> terrain_layer);
  cairo_surface_destroy(cont -> portal_layer);
  cairo_surface_destroy(cont -> face_layer);
  free(cont);
  pthread_mutex_destroy(&drawing_mutex);
}
