# 🎢 IBCLandia — Sistema de gestión de parque de diversiones

> Trabajo práctico — INF2223 Estructura de Datos · Grupo 5  
> Lenguaje: **ANSI C** (compatible con Turbo C)

---

## 📋 Descripción general

IBCLandia es un sistema de consola que modela la operación diaria de un parque de diversiones. El programa permite registrar, consultar, modificar y eliminar zonas temáticas, atracciones, visitantes y entradas, además de simular el funcionamiento real de las filas de espera y generar reportes al cierre del día.

El proyecto fue desarrollado como solución al problema de gestión dispersa y manual de un parque en crecimiento, aplicando estructuras de datos vistas en cátedra: **lista enlazada simple**, **árbol binario de búsqueda (ABB)** y **cola (queue) con punteros**.

---

## 🗂️ Estructuras de datos utilizadas

| Estructura | Aplicación en el sistema |
|---|---|
| Lista enlazada simple | Zonas temáticas del parque (`NodoZona`) |
| Árbol binario de búsqueda | Entradas ordenadas por código (`NodoEntrada`) |
| Cola con punteros (frente/final) | Fila de espera de cada atracción (`FilaEspera`) |
| Arreglos estáticos globales | Acceso rápido a zonas y atracciones para ordenamiento y estadísticas |

---

## 🏗️ Estructuras principales

```
Parque
 ├── listaZonas       → lista enlazada de NodoZona
 │    └── ZonaTematica
 │         └── atracciones[]  → arreglo de hasta 10 punteros a Atraccion
 │              └── FilaEspera → cola de Visitante
 └── arbolEntradas    → ABB de NodoEntrada (búsqueda por código)
```

Cada `Visitante` almacena: ID, nombre, edad, altura y peso.  
Cada `Atraccion` registra: código, nombre, estado, capacidad por ciclo, duración, restricciones (edad, altura, peso) y contador de atendidos en el día.  
Cada `NodoEntrada` registra: código, titular, tipo, precio y estado.

---

## ⚙️ Arranque del sistema

Al iniciar, el programa solicita el nombre del parque y ofrece cargar datos de prueba generados internamente (sin archivos externos):

- Genera **4 zonas temáticas** fijas: Reino Fantástico, Ciudad Futurista, Aventura Selvática y Mundo Infantil, cada una con su capacidad y horario.
- Genera **5 atracciones por zona** (20 en total), con capacidades de ciclo, duraciones y edades mínimas variadas.
- Genera **N visitantes** (número ingresado por el usuario) con semilla fija para reproducibilidad:
  - Crea una entrada por visitante en el ABB con tipo aleatorio (General, Familiar, Infantil o Rápido).
  - Las entradas con estado activo se convierten automáticamente a **utilizadas** al ingresar al parque.
  - Las entradas vencidas o anuladas se registran en el ABB pero no generan visitante.
  - Cada visitante activo es encolado en la fila de una atracción elegida aleatoriamente.
  - El contador `personasDentro` se incrementa por cada ingreso válido.

---

## 🎯 Operación durante el día

El menú principal se divide en **Agregar datos** y **Operar datos**.

### Agregar
- **Zona nueva**: ingreso guiado de código, nombre, temática, horario y capacidad; valida código duplicado en la lista enlazada y registra también en el arreglo global.
- **Atracción nueva**: requiere zona existente (hasta 10 atracciones por zona); valida código duplicado; inicializa su fila vacía y contador en cero; acepta restricciones de edad, altura y peso.
- **Visitante a fila**: muestra zonas disponibles, solicita código de atracción; verifica que esté operativa y que el visitante cumpla las restricciones; lo encola al final de la fila e incrementa `personasDentro`.
- **Entrada nueva**: ingreso de código, titular, tipo (con precio automático) y estado; se inserta en el ABB.

### Buscar
- Zona por código → búsqueda secuencial en la lista enlazada.
- Atracción por código → búsqueda secuencial en el arreglo global.
- Entrada por código → búsqueda en el ABB (O log n); muestra todos los datos del nodo.

### Modificar
- **Zona**: permite cambiar nombre, temática y capacidad (Enter mantiene el valor actual); el cambio se refleja directamente en el nodo de la lista.
- **Atracción**: permite cambiar nombre, capacidad por ciclo y duración del ciclo.
- **Estado de atracción**: cuatro estados posibles:
  - `Operativa` → reactiva la fila (fila retomada con las personas en espera).
  - `Mantenimiento` → suspende la fila sin vaciarla.
  - `Fuera de servicio` → vacía la fila liberando memoria.
  - `Cerrada por horario` → vacía la fila liberando memoria.

### Eliminar
- **Zona**: solo si no tiene atracciones asociadas; se elimina de la lista enlazada y del arreglo global (compactación por reemplazo con el último elemento).
- **Atracción**: vacía su fila liberando memoria; la elimina del arreglo interno de su zona (compactando hacia la izquierda) y del arreglo global.
- **Entrada**: solo si su estado es Anulada o Vencida; eliminación estándar de ABB con sucesor en orden.
- **Visitante de fila**: retiro individual por ID desde cualquier posición de la fila enlazada.

### Listar
- Todas las zonas con sus atracciones (estado, ciclo, personas en fila).
- Todas las entradas en recorrido **inorden** del ABB (orden ascendente por código).
- Fila completa de una atracción (posición, ID y nombre de cada visitante).
- Atracciones no operativas con su estado y tamaño de fila.

---

## 📊 Funciones del parque

| Función | Descripción |
|---|---|
| Calcular ingresos | Suma precios de todas las entradas con estado `Utilizada` recorriendo el ABB |
| Tiempo de espera | Calcula ciclos necesarios para vaciar una fila y tiempo de espera para una posición específica |
| Simular ciclos | Desencola grupos de visitantes ciclo a ciclo, imprime cada atendido, acumula `atendidosHoy` |
| Fila más larga | Recorre el arreglo global y reporta la atracción con mayor cantidad en fila |
| Más visitada del día | Reporta la atracción con mayor `atendidosHoy` (acumulado por simulaciones) |
| Ocupación por zona | Suma personas en fila de todas las atracciones de cada zona; ordena zonas de mayor a menor mediante **burbuja**; muestra alertas `[LLENA]` (≥ capacidad) y `[CASI LLENA]` (≥ 80 %) |
| Ordenar atracciones de zona | Ordena el arreglo interno de atracciones de una zona por código ascendente (**burbuja**) |

---

## 📝 Reporte de cierre del día

Al seleccionar el cierre se muestra un resumen consolidado:

```
========================================
    REPORTE DE CIERRE - IBCLANDIA
========================================
Entradas utilizadas   : N
Personas en el parque : N
Ingresos del dia      : $N

Atraccion mas visitada del dia:
  [cod] Nombre | Personas atendidas: N

Atraccion con fila mas larga:
  [cod] Nombre | Personas en espera: N

--- OCUPACION POR ZONA (mayor a menor) ---
  1. Nombre zona  | Fila: NNN | Capacidad: NNN [LLENA / CASI LLENA]
  ...
------------------------------------------
========================================
```

---

## 💰 Precios de entradas

| Tipo | Precio |
|---|---|
| General | $3.000 |
| Familiar | $5.000 |
| Infantil | $2.000 |
| Rápido | $6.000 |

---

## 🚀 Compilación y ejecución

```bash
# GCC
gcc -o ibclandia _Grupo_5.c
./ibclandia

# Turbo C / Borland
# Abrir _Grupo_5.c y compilar con Alt+F9
```

No requiere librerías externas ni archivos de datos. Al ejecutar, el propio sistema genera todos los datos de prueba de forma interna.

---

## 📁 Estructura del repositorio

```
📄 _Grupo_5.c     → código fuente completo (ANSI C, un solo archivo)
📄 README.md      → este archivo
```

---

## 👥 Integrantes

Grupo 5 — INF2223 Estructura de Datos
