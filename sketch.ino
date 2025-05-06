import speech_recognition as sr
import RPi.GPIO as GPIO
import tkinter as tk

# === GPIO SETUP ===
LED_PIN = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

# === GUI SETUP ===
root = tk.Tk()
root.title("Smart Home - Voice LED Control")
root.geometry("400x200")

status_label = tk.Label(root, text="LED Status: OFF", font=("Arial", 18))
status_label.pack(pady=20)

def update_gui(state):
    status_label.config(text=f"LED Status: {'ON' if state else 'OFF'}")
    root.update()

# === VOICE COMMAND FUNCTION ===
def listen_command():
    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        print("Listening for command...")
        status_label.config(text="Listening...")
        root.update()
        audio = recognizer.listen(source)

    try:
        command = recognizer.recognize_google(audio).lower()
        print("You said:", command)

        # Flexible keyword-based matching
        if "on" in command and "light" in command:
            GPIO.output(LED_PIN, GPIO.HIGH)
            update_gui(True)
            print("Command recognized: Turning ON the light.")
        elif "off" in command and "light" in command:
            GPIO.output(LED_PIN, GPIO.LOW)
            update_gui(False)
            print("Command recognized: Turning OFF the light.")
        else:
            print("Unknown command.")
            status_label.config(text="Unknown command.")
    except sr.UnknownValueError:
        print("Could not understand audio.")
        status_label.config(text="Could not understand audio.")
    except sr.RequestError as e:
        print(f"Speech recognition error: {e}")
        status_label.config(text=f"Speech error: {e}")

# === GUI BUTTONS ===
listen_button = tk.Button(
    root, 
    text="Listen for Command", 
    command=listen_command, 
    font=("Arial", 14)
)
listen_button.pack(pady=10)

exit_button = tk.Button(
    root, 
    text="Exit", 
    command=lambda: (GPIO.cleanup(), root.destroy()), 
    font=("Arial", 14)
)
exit_button.pack(pady=10)

# === Start with LED off ===
update_gui(False)
root.mainloop()
