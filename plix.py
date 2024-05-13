# reference code from: 
#   https://github.com/kkroening/ffmpeg-python/blob/master/examples/tensorflow_stream.py

import ffmpeg 
import numpy as np
import torch 
import torch.nn as nn
import os
import concurrent.futures
from threading import Lock

# OD_Model: nn.Module, object detection model
# BSD_Model: nn.Module, bumper sticker detection model

def start_video_decode(filename: str):
    args = (
        ffmpeg
        .input(in_filename)
        .output('pipe:', format='rawvideo', pix_fmt='rgb24')
        .compile()
    )
    return subprocess.Popen(args, stdout=subprocess.PIPE)

def read_frame(process, width, height):
    # decoded frame is in RGB888 format
    frame_size = width * height * 3
    in_bytes = process.stdout.read(frame_size)
    if len(in_bytes) == 0:
        frame = None
    else:
        assert len(in_bytes) == frame_size
        frame = (
            np
            .frombuffer(in_bytes, np.uint8)
            .reshape([height, width, 3])
        )
    return frame


def detect_bumper_stickers(frame):
    # assmung BSD_Model() returns 4 integers which points 
    # the (top, left, bottom, right) of the bounding box
    output = BSD_Model(frame)
    cars_with_BS = []
    for obj in output:
        cars_with_BS.append(obj)
    return cars_with_BS

def detect_cars(filename: str, width: int, height: int):
    video_decode_process = start_video_decode(filename)
    frame_idx: int = 0
    frames_with_cars: List[dict] = []
    while True:
        frame = read_frame(video_decode_process, width, height)
        if frame is None:
            break
        # assmung OD_Model() returns list of object index (e.g: car = 0, people = 1, ...) 
        # and 4 integers which points the (top, left, bottom, right) of the bounding box
        output = OD_Model(frame)
        cars = []
        for obj in output:
            if output.objectidx == 0: # if the object is a car
                cars.append(obj)
                cars_with_BS = detect_bumper_stickers(frame)
        if len(cars) > 0:
            frames_with_cars.append({frame_idx: 'frame_idx', cars: 'cars'})
        frame_idx = frame_idx + 1

    return frames_with_cars

def video_pipeline(video_bucket_path : str):
    files = [f for f in os.listdir(video_bucket_path) if os.path.isfile(f)]
    frames_with_cars_with_volcom = []
    list_mutex = Lock()

    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as executor:
        futures = []
        for file in files:
            try:
                probe = ffmpeg.probe(args.in_filename)
            except ffmpeg.Error as e:
                print(f'{file} is not a video file')
                continue
    
            futures.append(executor.submit(detect_cars, filename=file, width=width, height=height))
            for future in concurrent.futures.as_completed(futures):
                frames_with_cars = future.result()
                if frames_with_cars is None:
                    continue
                with list_mutex:
                    frames_with_cars_with_volcom.append(frames_with_cars)

    return frames_with_cars_with_volcom # List of frames (see Output)
