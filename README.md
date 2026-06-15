# STM32F401RC Bare-Metal Drivers

A bare-metal embedded firmware project for the **STM32F401RC** (ARM Cortex-M4),
built with [PlatformIO](https://platformio.org/) on the **CMSIS** framework — no
vendor HAL. All peripheral access is done through a custom, layered driver stack
written directly against the device registers.

## Hardware

| Item        | Value                          |
| ----------- | ------------------------------ |
| Board       | `genericSTM32F401RC`           |
| Core        | ARM Cortex-M4 (STM32F401RC)    |
| Framework   | CMSIS                          |
| Programmer  | ST-Link (upload + debug)       |

## Architecture

The codebase is organised in classic embedded layers. Each driver pairs an
implementation (`src/`) with its public interface (`include/`) and, where
relevant, a separate configuration file (`*_cfg.c` / `*_cfg.h`) so wiring and
pin assignments live apart from logic.

```
Application            src/main.c
        │
   OS / Scheduler      OS/sched          cooperative runnable scheduler
        │
   HAL (drivers)       led, switch, lcd, sevenseg, ledmatrix, hserial
        │
   MCAL (peripherals)  rcc, gpio, systick, nvic, uart, spi, dma
        │
   Hardware            STM32F401RC registers
```

### MCAL — Microcontroller Abstraction Layer (`src/mcal/`)

| Module    | Purpose                                              |
| --------- | ---------------------------------------------------- |
| `rcc`     | Clock control and per-peripheral clock enabling      |
| `gpio`    | Pin direction, mode, alternate-function, speed       |
| `systick` | SysTick timer, busy-wait and interrupt timing        |
| `nvic`    | Interrupt enable/disable and priority                |
| `uart`    | USART driver (polling, interrupt and DMA modes)      |
| `spi`     | SPI master driver                                    |
| `dma`     | DMA transfers (mem-to-mem, mem-to-peripheral)        |

### HAL — Hardware Abstraction Layer (`src/hal/`)

| Module      | Purpose                                              |
| ----------- | ---------------------------------------------------- |
| `led`       | LED on/off/toggle driver                             |
| `switch`    | Debounced push-button input                          |
| `lcd`       | Character LCD (sync and async APIs)                  |
| `sevenseg`  | Seven-segment display driver                         |
| `ledmatrix` | LED matrix display driver                            |
| `hserial`   | Unified serial interface abstracting UART / SPI      |

`hserial` exposes a single `HSerial_Interface_t` (`TransmitBuffer` /
`ReceiveBuffer`) so application code can talk to a UART or SPI backend through
the same function pointers.

### OS — Cooperative Scheduler (`src/OS/sched.c`)

A lightweight time-triggered scheduler. Register periodic tasks as `Runnable_t`
entries (callback, period, first delay, priority) and let the SysTick tick drive
them:

```c
Sched_Init(1);                       // 1 ms tick
Sched_RegisterRunnable(&my_runnable);
Sched_Start();
```

## Project Layout

```
Hello_arm/
├── platformio.ini       # board, framework, upload/debug config
├── include/             # public headers (mirrors src/ layout)
│   ├── mcal/  hal/  OS/
│   ├── std_types.h      # shared typedefs
│   └── macros.h         # shared helper macros
├── src/                 # driver implementations + main.c
│   ├── mcal/  hal/  OS/
│   └── main.c           # application entry point
├── lib/                 # private PlatformIO libraries
└── test/                # unit tests
```

## Build, Upload & Debug

Requires the [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation/)
(or the VS Code PlatformIO extension) and an ST-Link probe.

```bash
# Build the default environment
pio run

# Build and flash to the board over ST-Link
pio run -e genericSTM32F401RC -t upload

# Start a debug session
pio debug -e genericSTM32F401RC
```

## Current `main.c`

The active entry point configures GPIO PA5/PA6/PA7 for SPI1 alternate function,
initialises the `hserial` SPI backend, and continuously transmits a test buffer
over SPI. Numerous earlier experiments (LED scheduler, UART/DMA, LCD, seven-seg,
LED matrix) are kept as commented references in the same file.

## Conventions

- **Direct register access** — drivers write to `volatile` register macros; no
  vendor HAL is used.
- **Config split** — hardware wiring lives in `*_cfg.c/.h`, logic in the main
  driver file. Edit the config files to retarget pins/peripherals.
- **Layout** — public headers go in `include/`, implementations in `src/`,
  private libraries under `lib/<name>/src`, mirroring PlatformIO conventions.

---

Part of the **ITI Embedded** training work.
