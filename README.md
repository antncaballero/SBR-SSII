# Sistemas basados en reglas con encadenamiento hacia atrás  
## Sistemas inteligentes - Curso 24/25 - Universidad de Murcia  
### Objetivo de la práctica
El objetivo de esta práctica consiste en diseñar y construir un SBR incorporándole el conocimiento sobre la incertidumbre expresado por factores de certeza (FCs). Este sistema (SBR con FCs) nos permitirá  tomar decisiones en distintas situaciones. Para ello, diseñaremos (e implementaremos) un motor de inferencia con encadenamiento hacia atrás adaptado al nuevo conocimiento disponible.

### Compilación:  
```bash
make
```  
### Ejecución:  
```bash
SBR-FC.exe baseDeConocimiento baseDeHechos
```
### Formato de la base de hechos: 
La base de hechos contiene un conjunto de hechos iniciales y su respectivo factor asociado. El formato es el siguiente:
```
Nº Hechos  
hecho1, factor1    
...  
Objetivo    
hechoObjetivo
```
### Formato de la base de conocimientos:
La base de conocimientos contiene un conjunto de reglas con identificadores únicos, cada una asociada a un factor. El formato es el siguiente:  
```
Nº Reglas  
Identificador1: Regla1, factor1  
Identificador2: Regla2, factor2 
... 
```

### Fichero salida:
Se generará un archivo llamado `salidaLog_BaseConocimientoBaseHechos.txt` con información sobre el proceso de inferencia y cálculos intermedios. 

### Casos de prueba
Se proporcionan varios casos de prueba con bases de hechos y conocimiento en la carpeta `CasosDePrueba`

### Cómo Usar el Sistema

1. **Preparar la Base de Hechos**: Crear un archivo con el formato especificado en la sección [Formato de la Base de Hechos](#formato-de-la-base-de-hechos).
2. **Definir la Base de Conocimientos**: Crear un archivo con el formato especificado en la sección [Formato de la Base de Conocimientos](#formato-de-la-base-de-conocimientos).
3. **Compilar el programa**: Compilar el programa con make.
4. **Ejecutar el Sistema**: Ejecutar el programa para iniciar el proceso de inferencia.
5. **Consultar el Registro**: Revisar el archivo `salidaLog_BaseConocimientoBaseHechos.txt` para analizar el proceso y los resultados obtenidos.