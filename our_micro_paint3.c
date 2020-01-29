#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct t_zona
{
	int width;
	int height;
	char fondo;
}		s_zona;

typedef struct t_forma
{
	char tipo;
	float x;
	float y;
	float width;
	float height;
	char color;
}		s_forma;

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int ft_error(FILE *file, char *dibujo, char *error)
{
	if (file)
		fclose(file);
	if (dibujo)
		free(dibujo);
	if (error)
		write (1, error, ft_strlen(error));
	return (1);
}

void ft_draw(char *dibujo, s_zona zona)
{
	int i = 0;
	while (i < zona.height)
	{
		write (1, dibujo + (i * zona.width), zona.width);
		write (1, "\n", 1);
		i++;
	}
}

char *get_zona(FILE *file, s_zona *zona)
{
	int i = 0, scan_ret;
	char *dibujo;

	if ((scan_ret = fscanf(file, "%d %d %c\n", &zona->width, &zona->height, &zona->fondo)) != 3)
		return (0);
	if (zona->width <= 0 || zona->width > 300 || zona->height <= 0 || zona->height > 300)
		return (0);
	if (!(dibujo = (char *)malloc(sizeof(*dibujo) * (zona->width * zona->height))))
		return (0);
	while (i < zona->width * zona->height)
		dibujo[i++] = zona->fondo;
	return (dibujo);
}


int in_rectangle(float x, float y, s_forma forma)
{
	if (x < forma.x || forma.x + forma.width < x || y < forma.y || forma.y + forma.height < y)
		return (0);
	if (x - forma.x < 1 || forma.x + forma.width - x < 1 || y - forma.y < 1 || + forma.y + forma.height - y < 1 )
		return (2);
	return (1);
}

int	get_forma(FILE *file, char **dibujo, s_zona zona)
{
	int x = 0, y = 0, ret, scan_ret;
	s_forma forma;
	while ((scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &forma.tipo, &forma.x, &forma.y, &forma.width, &forma.height, &forma.color))== 6)
	{
		if (forma.width <= 0 || forma.height <= 0 || (forma.tipo != 'r' && forma.tipo != 'R'))
			return (0);
		x = 0;
		while (x < zona.height)
		{
			y = 0;
			while (y < zona.width)
			{
				ret = in_rectangle(y, x, forma);
				if ((forma.tipo == 'r' && ret == 2) || (forma.tipo == 'R' && ret))
					(*dibujo)[x * zona.width + y] = forma.color;
				y++;
			}
			x++;
		}
	}
	if (scan_ret != -1)
		return (0);
	return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	s_zona zona;
	char *dibujo;
	if (argc != 2)
		return (ft_error(NULL, NULL, "Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (ft_error(NULL, NULL, "Error: Operation file corrupted\n"));
	if (!(dibujo = get_zona(file, &zona)))
		return (ft_error(file, NULL, "Error: Operation file corrupted\n"));
	if (!(get_forma(file, &dibujo, zona)))
		return (ft_error(file, NULL, "Error: Operation file corrupted\n"));
	ft_draw(dibujo, zona);
	ft_error(file, dibujo, NULL);
	return (0);
}