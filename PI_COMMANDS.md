On previous released of rasbian, the pwm_2chan overlay could be used to map audio to PWM
pins. This however has changed since Buster and the correct overlay for audio is now
audremap

Insert the following into /boot/config.txt:
```
dtoverlay=audremap,pins_12_13,swap_lr
```
In my breadboard setup I wired the left and right channels in reverse, this can be remedied
in software by using the `swap_lr` parameter in the overlay. You can find more information
on the overlay's usage in /boot/overlay/README.md

Enable overscan too



