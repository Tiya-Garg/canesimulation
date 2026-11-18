import cv2
import serial
import time
from ultralytics import YOLO

# Safe connection attempt with fallback for demo purposes
ser = None
try:
    print("Connecting to Wokwi ESP32 simulation...")
    ser = serial.serial_for_url('rfc2217://localhost:4000', baudrate=9600, timeout=0.1)
    print("Connected to Wokwi successfully!")
except Exception as e:
    print(f"Warning: Could not connect to Wokwi ({e}). Running in Video-Only Simulation Mode.")

print("Loading YOLOv8 model for Smart Cane AI Core...")
model = YOLO("yolov8n.pt")

cap = cv2.VideoCapture("./video4.mp4")
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

left_boundary = int(width * 0.35)
right_boundary = int(width * 0.65)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    
    results = model(frame, conf=0.4, verbose=False)
    annotated_frame = results[0].plot()
    
    left_hazard, right_hazard, stairs_detected = False, False, False
    
    for box in results[0].boxes:
        xyxy = box.xyxy[0].cpu().numpy()
        x_min, y_min, x_max, y_max = xyxy
        x_center = (x_min + x_max) / 2
        
        if x_center < left_boundary:
            left_hazard = True
        elif x_center > right_boundary:
            right_hazard = True
        else:
            stairs_detected = True

    esp_signal = 'C'
    if stairs_detected: esp_signal = 'S'
    elif left_hazard: esp_signal = 'L'
    elif right_hazard: esp_signal = 'R'

    # Send if connected
    esp_feedback = "SIMULATED_HW_OK"
    if ser and ser.is_open:
        try:
            ser.write(esp_signal.encode())
            if ser.in_waiting > 0:
                line = ser.readline()
                if line:
                    esp_feedback = line.decode('utf-8', errors='ignore').strip()
        except:
            pass

    print(f"Sent: [{esp_signal}] ➔ ESP32 Feedback: [{esp_feedback}]")

    # Render HUD
    hud_text = f"ESP32 Signal: {esp_signal} | Status: {esp_feedback}"
    cv2.putText(annotated_frame, hud_text, (30, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    
    cv2.imshow("Smart Cane Simulation", annotated_frame)
    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
if ser and ser.is_open:
    ser.close()