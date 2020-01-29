#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct s_zona
{
	int width;
	int height;
	char fondo;
}				t_zona;

typedef struct s_forma
{
	char tipo;
	float x;
	float y;
	float radius;
	char color;
}				t_forma;

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

void	ft_draw(char *dibujo , t_zona zona)
{
	int i = 0;
	while (i < zona.height)
	{
		write (1, dibujo + (i * zona.width) , zona.width);
		write (1, "\n", 1);
		i++;
	}
}

char	*get_zona(FILE *file, t_zona *zona)
{
	char *dibujo;
	int i =  0;
	if ((fscanf(file, "%d %d %c\n", &zona->width, &zona->height, &zona->fondo)) != 3)
		return (0);
	if (zona->width <= 0 || zona->width > 300 || zona->height <= 0 || zona->height > 300)
		return (0);
	if (!(dibujo = (char *)malloc(sizeof(*dibujo) * (zona->width * zona->height))))
		return (0);
	while (i < zona->width * zona->height)
		dibujo[i++] = zona->fondo;
	return (dibujo);
}

int	in_circle(float x, float y, t_forma forma)
{
	float	distance;

	distance = sqrtf(powf(x - forma.x, 2) + powf(y - forma.y, 2));
	if (distance <= forma.radius)
	{
		if ((forma.radius - distance) < 1)
			return (2);
		return (1);
	}
	return (0);
}

int	get_forma(FILE *file, char **dibujo, t_zona zona)
{
	t_forma forma;
	int x = 0, y = 0, ret, ret_scan;

	while ((ret_scan = fscanf(file, "%c %f %f %f  %c\n", &forma.tipo, &forma.x, &forma.y, &forma.radius, &forma.color)) == 5)
	{
		if (forma.radius <= 0  || (forma.tipo != 'c' && forma.tipo != 'C'))
			return (0);
			x = 0; 
		while (x < zona.height)
		{
			y = 0;
			while (y < zona.width)
			{
				ret = in_circle(y, x, forma);
				if ((forma.tipo == 'c' && ret == 2) || (forma.tipo == 'C' && ret))
					(*dibujo)[x * zona.width + y] = forma.color;
				y++;
			}
			x++;
		}
	}
	if (ret_scan != -1)
		return (0);
	return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	char *dibujo;
	t_zona zona;

	if (argc != 2)
		return (ft_error(NULL, NULL , "Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (ft_error(NULL, NULL , "Error: Operation file corrupted\n"));
	if (!(dibujo = get_zona(file, &zona)))
		return (ft_error(file, NULL , "Error: Operation file corrupted\n"));
	if (!(get_forma(file, &dibujo, zona)))
		return (ft_error(file, NULL , "Error: Operation file corrupted\n"));
	ft_draw(dibujo, zona);
	ft_error(file, dibujo, NULL);
	return (0);
}
