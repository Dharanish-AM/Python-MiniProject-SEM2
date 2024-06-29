import cv2
import numpy as np
from collections import deque
from datetime import datetime

bg_subtractor = cv2.createBackgroundSubtractorMOG2(history=500, varThreshold=100, detectShadows=True)
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
cap = cv2.VideoCapture("27260-362770008_small.mp4")

frame_vehicle_counts = deque()
frame_time = deque()
interval_seconds = 60
traffic_data = []

roi_top = 200
roi_bottom = 400
max_distance = 50
contour_area_threshold = 500
tracked_vehicles = []
total_vehicles_passed = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break

    fg_mask = bg_subtractor.apply(frame)
    _, fg_mask = cv2.threshold(fg_mask, 200, 255, cv2.THRESH_BINARY)
    fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_OPEN, kernel)
    fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_CLOSE, kernel)
    contours, _ = cv2.findContours(fg_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    current_vehicle_count = 0
    new_tracked_vehicles = []

    for contour in contours:
        if cv2.contourArea(contour) > contour_area_threshold:
            x, y, w, h = cv2.boundingRect(contour)
            centroid = np.array([x + w // 2, y + h // 2])

            if roi_top < centroid[1] < roi_bottom:
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                matched_vehicle = None
                min_distance = float('inf')

                for tracked_vehicle in tracked_vehicles:
                    distance = np.linalg.norm(tracked_vehicle['centroid'] - centroid)
                    if distance < max_distance and distance < min_distance:
                        matched_vehicle = tracked_vehicle
                        min_distance = distance

                if matched_vehicle is None:
                    new_vehicle = {'centroid': centroid, 'last_seen': datetime.now()}
                    new_tracked_vehicles.append(new_vehicle)
                    current_vehicle_count += 1
                    total_vehicles_passed += 1
                else:
                    matched_vehicle['centroid'] = centroid
                    matched_vehicle['last_seen'] = datetime.now()

    current_time = datetime.now()
    frame_vehicle_counts.append(current_vehicle_count)
    frame_time.append(current_time)

    if current_vehicle_count > 0:
        print(f"Time: {current_time}, Current Passing: {current_vehicle_count}")
    else:
        print(f"Time: {current_time}, Current Passing: No vehicles passed")

    cv2.putText(frame, f"Current Passing: {current_vehicle_count}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    cv2.imshow('Vehicle Detection', frame)
    cv2.imshow("Mask", fg_mask)

    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
