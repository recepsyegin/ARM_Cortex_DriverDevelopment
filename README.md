This repository is a from-scratch peripheral driver library written entirely in C for ARM Cortex-M4 based microcontrollers. No HALs, no CMSIS Drivers — just raw, register-level access and full control over the hardware.

This project is for embedded engineers, students, and low-level enthusiasts who want to understand what truly happens under the hood when developing firmware for STM32 and other Cortex-M devices.

# 🚀 ARM Cortex-M4 Bare-Metal Driver Development

Welcome to a deep dive into **bare-metal embedded systems development**! This repository is a **from-scratch peripheral driver library** written entirely in C for **ARM Cortex-M4** based microcontrollers. No HALs, no CMSIS Drivers — just raw, register-level access and full control over the hardware.

This project is for embedded engineers, students, and low-level enthusiasts who want to **understand what truly happens under the hood** when developing firmware for STM32 and other Cortex-M devices.

---

## 🧠 Why This Project Exists

Most developers rely on vendor-supplied HAL libraries. But they often obscure what's going on behind the scenes. This project strips that all away — you will:

- Learn how each peripheral works at the **register level**
- Understand the **startup flow** of a microcontroller
- Write **modular, reusable drivers**
- Develop a real appreciation for the architecture and power of ARM Cortex-M processors

This isn't just a codebase — it's a **learning experience**.

---

## 📦 What's Inside?

The core of the project is located in `PROJECT_DRIVERS/Src/`, where each peripheral is implemented from scratch with its own driver code.

| Driver | Description |
|--------|-------------|
| **GPIO**   | General-purpose digital I/O (direction, pull-up/down, read/write) |
| **RCC**    | Clock configuration and peripheral enablement |
| **NVIC**   | Nested interrupt controller configuration, priorities |
| **EXTI**   | External interrupt management via GPIO |
| **UART**   | Register-level USART communication (TX/RX, config) |
| **SPI**    | SPI master mode communication |
| **I2C**    | I2C master communication with clock, address, and ACK control 







