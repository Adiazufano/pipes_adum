#include "printf/ft_printf.h"

/*
acces se usa para verificar si un proceso
tiene permisos de acceso a un archivo o directorio especifico

su prototipo es int access(const char *path, int mode)

path: es una cadena que indica la ruta del archivo o directorio a comprobar
mode: es un entero que verifica el tipo de permiso que se desea verificar los mas comunes son:

F_OK -> verirfica si existe el archivo o directorio.
R_OK -> Verifica el permiso de lectura.
W_OK -> Verifica los permisos de escritura.
X_OK -> Verifica permiso de ejecucion

La funcion devuelve 0 si el acceso esta permitido seun el permiso solicitado y -1
si no lo esta o ocurre un error.
En caso de error , se puede consultar la variable global errno
para obtener detalles sobre el motivo.
*/

/* int	main()
{
	const char *filename = "hola";
	if (access(filename, F_OK) == 0)
	{
		if (access(filename, W_OK) == 0)
			ft_printf("El fichero existe y acepta lectura\n");
		else
			ft_printf("El fichero existe y no acepta lectura\n");	
		if (access(filename, R_OK) == 0)
			ft_printf("El fichero existe y acepta escritura\n");
		else
			ft_printf("El fichero existe y no acepta escritura\n");
		if (access(filename, X_OK) == 0)
			ft_printf("El fichero existe y acepta ejecucion\n");
		else
			ft_printf("El fichero existe y no acepta escritura\n");
	}
	else
		ft_printf("El fichero no existe\n");
} */

/*
La funcion dup2 se usa para duplicar un fd que ya existe
y asignarlo a otro especifico permite redidirgir operaciones de entrada salida.

int dup2(int oldfd, int newfd);
oldf -> es el fd que ya existe y se quiere duplicar.
newfd -> Entero que espeifica nuevo descriptor. Si ya esta abierto
lo cierra antes de duplicarlo. 

Duplicacion: ambos fd apuntan al mismo archivo, comparten atributos
como posicion del cursor y bloqueos

Redireccion: Util para cambiar destinos de salida y entrada
por ejemplo redirigir stdout a un archivo.

Si es exitoso retorna newfd.
si falla da -1(si oldfd no es valido o newfd supera el limite del sistema).abort

*/

/* int	main(void)
{
	int	archivo = open("hola.txt", O_WRONLY | O_CREAT, 0644);
	ft_printf("%i",dup2(archivo, STDOUT_FILENO));
	printf("ADIO");
	close(archivo);
	return (0);
} */
/*
La funcion fork es un allamada al sistema para crear un proceso nuevo conocido como 
proceso hijo que es casi una copia del proceso que lo invoca(proceso padre).
Ambos procesos se ejecutan de forma concurrente a partir de la siguiente intruaccion de fork()

Duplicado del proceso: Al llamar a fork crea un duplicado del proceso actual incluyendo
variables , archivos abiertos y estados de ejecucion.

Valor de retorno: 
	Al proceso padre, fork() le devuleve el PID (Identificador de proceso) del hijo.
	Al procesp hijo, fork() le devuelve 0.
	si ocurrre un error devuelve -1 en ambos.

Diferenciacion: para saber si el codigo se ejcuta en el padre o el hijo, se verifica el valor
que devuelve fork() con un if.

Aunque ambos procesos comienzan con el mismo contenido de memoria , a partir de la bifurcacion
son independientes los cambios en uno no afecatan al otro.

el hijo hereda de los descriptores de archivos del padre , lo que permite , compartir pipes o redirigir salidas.abort

Unproceso hijo puede crear mas procesos hijos
*/

/*
#include <sys/wait.h>

int	main(void)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		ft_printf("Soy el hijo, PID=%d\n", getpid());
	}
	else if (pid > 0)
	{
		wait(NULL);
		ft_printf("Soy el padre, PID=%d, hijo=%d\n", getpid(), pid);
	}
	else
	{
		ft_printf("Error al crear el proceso\n");
	}
	return (0);
}
*/


/*
Un pipe es un mecanismo que permite comunicar 2 procesoso generalmente relacionados,
en SO como linux es para que un proceso mande datos a otro de forma eficiente y sencilla
usando un canala unidireccional

Un pipe crea un flujo de datos en memoria, donde un extremo se pasa para escribri y 
otro para leer.
Por lo general se usa para que un proceso padre y sus hijos se comuniquen: 
uno escribe datos en el pipe y el otro los lee.
El flujo es unidireccional: los datos van en una sola direccion(de escritor a lector).
El pipe se implementa en el sistema como un bufer temporal en memoria.abort

se crea asi:
int fd[2];
pipe(fd);
fd es el descriptor de archivo para leer el pipe.
fd[1]: es el descriptor de archivo para escribir en el pipe.
*/

/* int	main(void)
{
	int fd[2];
	pipe(fd);

	if (fork() == 0)
	{
		close(fd[1]);
		char buffer[100];
		read(fd[0],buffer,sizeof(buffer));
		ft_printf("Hijo leyo: %s\n", buffer);
		close(fd[0]);
	}
	else
	{
		close(fd[0]);
		ft_putstr_fd("Hola desde el padres", fd[1]);
		close(fd[1]);
	}
	return (0);
} */

/*
execve es una llamada al sistema de linux que remplaza el proceso actual por un nuevo 
programa es decir el proceso que llama a execve deja de ejecutar su codigo y ejecuta
el programa especificado en la llamada

int execve(const char *pathname, char *const argv[], char *const envp[]);

pathname: es la ruta l ejecutable que queremos ejecutar.
argv: Array de cadenas de texto con los argumentos del programa
envp: Array de cadenas de texto con las variables de entorno del programa.abort

El proceso actual se remplaza: el codigo, datos , pilas ... del proceso qeu llama a execve
se sustituyen por los nuevos del programa

PID se mantiene: el identificador del proceso no cambia pero si el contenido.

No regresa: Si execve tiene exito no retorna , si falla retorna -1 y se puede consultar errno
para ver el motivo.
*/

/* int	main(void)
{
	char *argv[] = {"/bin/ls", "-l", NULL};
	char *envp[] = {NULL};

	if (execve("/bin/ls", argv, envp) == -1)
	{
		perror("execve");
	}
	return (1);
} */