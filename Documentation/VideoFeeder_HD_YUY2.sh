#!/bin/bash

sleep 2

echo "nvidia" | sudo -S echo "" 2>&1

sudo modprobe v4l2loopback devices=1 video_nr=10 max_buffers=2 exclusive_caps=1

