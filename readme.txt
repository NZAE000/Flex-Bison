Control 1 : Lenguajes de programación
Herramienta: "Flex y Bison"

En este proyecto se encuentra el desarrollo del Control 1, los directorios se organizan de la siguiente forma:

- En la carpeta "src" se encuentran todos los archivos fuente. 
- Cada pregunta cuenta con su carpeta correspondiente denominada como "questionN", con N desde 1 a la 4.
- Dentro de cada carpeta questionN se encuentra el archivo lexer y parser.

Para construir y ejecutar el programa se utiliza el siguiente comando:		

		make Q=questionN && ./draw

	Ejemplo:

		make Q=question1 && ./draw

Para ejecutar otra pregunta se debe limpiar el programa de la siguiente forma:

		make cleanall Q=questionN

	Ejemplo:

		make cleanall Q=question4

* Si no se especifica la opcion Q, tanto en construir como limpiar, se realiza la pregunta 4 por defecto.
