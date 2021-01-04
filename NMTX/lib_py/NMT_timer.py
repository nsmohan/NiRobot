#!/usr/bin/env python
"""
"  @file      NMT_timer.py
"  @brief     Library to repeat action at given interval
"  @details   Facility which allows user to repeat action at a given interval
"  @author    Nitin Mohan
"  @date      Sept 30, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from threading import Timer

""" 
"  @class  Timer
"          Timer Object
"""
class NMT_Timer():
    
    def __init__(self, interval, function, *args, **kwargs):
        self._timer     = None
        self.interval   = interval
        self.function   = function
        self.args       = args
        self.kwargs     = kwargs
        self.is_running = False

    def _run(self):
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False


