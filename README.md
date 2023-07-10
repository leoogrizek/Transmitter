# Remote-Controlled Airplane Transmitter

## Description
This repository contains the code and documentation for a remote-controlled airplane transmitter built using an Arduino Nano and a NRF24L01 transceiver. The transmitter is part of a larger project to create a remote-controlled airplane.

## Table of Contents
- [Electronics](#electronics)
- [Installation](#installation)
- [Usage](#usage)

## Electronics
For information regarding the electronics and other details, refer to the `RC_airplane_english.pdf` file in this repository. For this part, you will need the following tools:

- Soldering station
- Wire cutters/strippers
- Crimping tool for 2.54mm connectors

The components should be attached to a base. In this project, plywood was used as the controller base.

## Installation
To get this project up and running, you will need to have Microchip Studio installed on your computer. You can download it from [here](https://www.microchip.com/mplab/microchip-studio). After installing Microchip Studio, clone this repository and add the `Transmitter` folder to the project or open the `Transmitter.atsln` file. AVR LIBM is necessary and should be installed within Microchip Studio by default.

## Usage
Build the project using the GNU compiler for AVR (atmega328p) in Microchip Studio and upload the code to your Arduino Nano. Make sure to connect the NRF24L01 transceiver and other components as described in `RC_airplane_english.pdf`.

