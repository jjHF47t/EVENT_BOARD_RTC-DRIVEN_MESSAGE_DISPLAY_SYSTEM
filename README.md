# 📅 RTC-Driven Smart Event Board
# Overview

The **RTC-Driven Smart Event Board** is an embedded system developed using the **LPC2148 ARM7 microcontroller** that automatically displays scheduled event messages based on the current date and time. It utilizes the Real-Time Clock (RTC) to continuously monitor time and activate predefined events without user intervention.

The system features a **16×2 LCD** for displaying the current time, date, day, temperature, and scrolling event messages. A built-in **administrator mode**, accessible through a keypad, allows authorized users to manage event schedules. Additionally, an ADC-based temperature sensor provides real-time temperature monitoring, while LED indicators reflect the system's operating state.

Designed with a modular architecture in Embedded C, the project demonstrates real-time scheduling, peripheral interfacing, and automation, making it suitable for applications such as digital notice boards, office reminders, classroom announcements, and industrial information displays.

# Objective

The primary objective of the **RTC-Driven Smart Event Board** is to design and implement an embedded system that automatically displays scheduled event messages based on real-time clock data, eliminating the need for manual updates.

The project aims to:

* Develop a real-time event display system using the LPC2148 microcontroller.
* Automatically monitor the current time and activate scheduled events at the appropriate time.
* Display the current time, date, day, and temperature when no event is active.
* Provide a scrolling message display for active events on a 16×2 LCD.
* Enable administrator access through a keypad interface for managing event schedules.
* Indicate system status using LED indicators for normal and event modes.
* Demonstrate integration of RTC, ADC, LCD, GPIO, and keypad peripherals in an embedded application.
* Create a reliable, low-cost, and easily expandable solution for smart notice boards and automated information display systems.

Block Diagram:

<img width="1536" height="1024" alt="Block Diagram " src="https://github.com/user-attachments/assets/5f76ae5b-c5b3-4ef9-8ca7-ef46996d8d0b" />


## 🚀 Features

* ⏰ Real-Time Clock (RTC) based event scheduling
* 📢 Automatic display of active event messages
* 🔄 Scrolling text for long event descriptions
* 📅 Live display of current time, date, and day
* 🌡️ Temperature monitoring using ADC
* 🔐 Admin mode accessed through long key press
* ⌨️ Keypad interface for event management
* 💡 LED indication for normal and active event states
* 📋 Displays the next upcoming event when no event is active
* 🕒 Automatic event duration calculation

## 🛠️ Hardware Used

* LPC2148 (ARM7TDMI-S) Microcontroller
* 16×2 Character LCD
* Matrix Keypad
* RTC Module (or LPC2148 internal RTC)
* Temperature Sensor connected to ADC
* Red LED
* Green LED
* Power Supply

## 📁 Project Structure

```
├── main.c
├── rtc.c / rtc.h
├── lcd.c / lcd.h
├── KPM.c / KPM.h
├── delay.c / delay.h
├── mini_project_functions.c / .h
├── lcd_defines.h
└── other supporting files
```

## ⚙️ Working

1. Initializes the LCD, keypad, ADC, GPIO, and RTC.
2. Reads the current time and date from the RTC.
3. Reads the temperature through the ADC and converts it for display.
4. Checks whether any scheduled event is currently active.
5. If an event is active:

   * Displays the scrolling event message on the LCD.
   * Turns on the Green LED.
6. If no event is active:

   * Displays the current time, date, day, and temperature.
   * Turns on the Red LED.
7. At the start of each new minute, shows the next scheduled event if none is currently active.
8. Detects a long keypad press to enter the administrator menu for event management.

## 📺 LCD Display Example

### Normal Mode

```
10:45:12     MON
15/06/2026  29°C
```

### Event Mode

```
EVENT ACTIVE
Project Review
```

### Next Event Preview

```
NEXT EVENT
Meeting 11:00
```

## 🔧 Software Requirements

* Keil µVision IDE
* Embedded C
* Flash Magic (for programming)
* Proteus (optional for simulation)

## 🎯 Applications

* College and university notice boards
* Office meeting reminders
* Factory production schedules
* Conference room displays
* Digital information boards
* Smart classroom announcements

## 🔮 Future Improvements

* EEPROM storage for persistent events
* UART or Bluetooth configuration
* Wi-Fi or IoT-based remote scheduling
* Buzzer notifications
* Touchscreen user interface
* Mobile app integration
  
## Conclusion

The **RTC-Driven Smart Event Board** successfully demonstrates how an embedded system can automate the display of scheduled messages using real-time clock functionality. By integrating the LPC2148 microcontroller with an LCD, keypad, ADC-based temperature sensing, and event scheduling logic, the system provides timely and dynamic information without manual intervention.

The project showcases key embedded systems concepts such as peripheral interfacing, real-time processing, GPIO control, ADC usage, and user interaction through an administrator mode. Its modular design makes it easy to extend with additional features such as EEPROM storage, wireless communication, or IoT connectivity, making it a practical foundation for smart notice boards and automated announcement systems.
