# Edge Impulse - OpenMV Object Detection Example

import sensor, image, time, os, tf, math, uos, gc
from pyb import I2C

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.

net = None
labels = None
min_confidence = 0.5

# Update with the new labels in the same order your model was trained.
labels = [
    "apple_fresh", 
    "apple_overripe", 
    "banana_fresh", 
    "banana_overripe", 
    "empty"
]

try:
    # Load built in model
    labels, net = tf.load_builtin_model('trained')
except Exception as e:
    raise Exception(e)

colors = [ # Add more colors if you are detecting more than 7 types of classes at once.
    (255,   0,   0), # Red for Apple Fresh
    (  0, 255,   0), # Green for Apple Overripe
    (255, 255,   0), # Blue for Banana Fresh
    (  0,   0, 255), # Yellow for Banana Overripe
    (255,   0, 255), # Magenta for Empty
    (  0, 255, 255),
    (255, 255, 255),
]

# I2C Setup - Initialize I2C as Master
i2c = I2C(2, I2C.MASTER)  # Use I2C port 2 as master
feather_addr = 0x08  # Feather M0 I2C address


# Fruit state constants for I2C communication
APPLE_FRESH = 0
APPLE_OVERRIPE = 1
BANANA_FRESH = 2
BANANA_OVERRIPE = 3
EMPTY = 4

# Clock for measuring FPS
clock = time.clock()

# Initialize the timer for 5-second intervals
last_communication_time = time.ticks_ms()  # Get the current time in milliseconds

def send_fruit_state_to_feather(state):
    """ Send the detected fruit state to Feather M0 via I2C """
    try:
        i2c.send(state, feather_addr)
        print(f"Sent state {state} to Feather")
    except OSError as e:
        print(f"Error sending data: {e}")




while(True):
    clock.tick()

    img = sensor.snapshot()

    # detect() returns all objects found in the image (splitted out per class already)
    # we skip class index 0, as that is the background, and then draw circles of the center
    # of our objects

    # Variable to track detected fruit state
    fruit_state = EMPTY  # Default to 'empty'


    for i, detection_list in enumerate(net.detect(img, thresholds=[(math.ceil(min_confidence * 255), 255)])):
        if (i == 0): 
            continue # background class
        if (len(detection_list) == 0): 
            continue # no detections for this class?

        print("********** %s **********" % labels[i])
        for d in detection_list:
            [x, y, w, h] = d.rect()
            center_x = math.floor(x + (w / 2))
            center_y = math.floor(y + (h / 2))
            print('x %d\ty %d' % (center_x, center_y))
            img.draw_circle((center_x, center_y, 12), color=colors[i], thickness=2)

        # Update fruit state based on the detected class
            if labels[i] == "apple_fresh":
                fruit_state = APPLE_FRESH
            elif labels[i] == "apple_overripe":
                fruit_state = APPLE_OVERRIPE
            elif labels[i] == "banana_fresh":
                fruit_state = BANANA_FRESH
            elif labels[i] == "banana_overripe":
                fruit_state = BANANA_OVERRIPE
            else:
                fruit_state = EMPTY

    # Check if 5 seconds (5000 ms) have passed since the last communication
    if time.ticks_diff(time.ticks_ms(), last_communication_time) >= 5000:
        # Send the detected fruit state to Feather M0 via I2C every 5 seconds
        send_fruit_state_to_feather(fruit_state)

        # Update the last communication time
        last_communication_time = time.ticks_ms()


    print(clock.fps(), "fps", end="\n\n")
