# Aquifera Waterflow

The official firmware for Aquifera Waterbox. Made to count flow of water from Aquifera watersystems.

This project can be used to send sms messages with the SIM800L V2 module (not the SIM800L V1, nor any other type of module guaranteed to succeed).

## How To:
### Preparation:

- Install [Arduino IDE](https://www.arduino.cc/en/main/software), or any IDE that can compile with the Arduino Framework
- Do correct wiring (detailed description below)

### Program it!:

1. Create **credentials.h**, save within the same folder. Copy this code inside **credentials.h**, and change the number inside `dest_phone_no[16]`:
```cpp
char dest_phone_no[16] = "+YYXXXXXXXXXXX";

// Change YY with country code, change XXX with phone number
// Example:
// char dest_phone_no[16] = "+6282121212121";
```

---

### Notes:

[Wiring/Cabling SIM800L V2](https://github.com/royyandzakiy/aquifera-waterflow-sms)

---

### a note from Royyan

a :droplet: Aquifera Project

Use this as you wish! Happy making! :octocat: