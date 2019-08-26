#!/usr/bin/env python
import struct
import socket
import time, threading

from core import eBPFCoreApplication, set_event_handler, FLOOD
from core.packets import *
import time


class SimpleSwitchApplication(eBPFCoreApplication):

    @set_event_handler(Header.HELLO)
    def hello(self, connection, pkt):
        self.mac_to_port = {}
        self.counter = {}

        with open('../examples/foo_counter.o', 'rb') as f:
            print("Installing the eBPF ELF")
            connection.send(InstallRequest(elf=f.read()))

        self.connection = connection
        self.last = 0


    @set_event_handler(Header.NOTIFY)
    def notify_event(self, connection, pkt):
        num_packets = struct.unpack('<I', pkt.data)
        print ("counter: {0:4d} packet(s)".format(num_packets[0]))


if __name__ == '__main__':
    SimpleSwitchApplication().run()
