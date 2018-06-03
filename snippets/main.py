'''
Created on 25-07-2017
'''
import time
import MDM
import SER2

GPIO_SDA = 2
GPIO_SCL = 3
CODEC_ADDR = 0x30

config = {
    0x04: 0x00,
    0x05: 0x10,
    0x06: 0x90,
    0x07: 0x00,
    0x08: 0x10,
    0x09: 0x08,
    0x0a: 0x00,
    0x0b: 0x00,
    0x0c: 0x00,
    0x0d: 0x33,
    0x0e: 0x0c,
    0x0f: 0x0c,
    0x10: 0x09,
    0x11: 0x09,
    0x12: 0x24,
    0x13: 0x24,
    0x14: 0x40,
    0x15: 0x00,
    0x16: 0x00,
    0x17: 0x8a
}


def contains_any(string, *substrings):
    return any(substring in string for substring in substrings)


def wait_for_response(timeout=5.0):
    data = ''
    time_max = time.time() + timeout
    while not contains_any(data, 'OK\r', 'ERROR\r') and time.time() < time_max:
        byte = MDM.readbyte()
        if byte != -1:
            data += chr(byte)

    return 'OK\r' in data


def write_registers(start_reg, *data):
    SER2.send('writing: %02x %r\r\n' % (start_reg, data))
    
    MDM.send('AT#I2CWR=%d,%d,%02x,%02x,%d\r' % (GPIO_SDA, GPIO_SCL, CODEC_ADDR, start_reg, len(data)), 0)
    MDM.send(''.join(hex(x)[2:].zfill(2) for x in data), 0)
    MDM.sendbyte(0x1a, 0)
    
    return wait_for_response()


def read_registers(start_reg, n):
    SER2.send('reading: %02x %d\r\n' % (start_reg, n))
    MDM.send('AT#I2CRD=%d,%d,%02x,%02x,%d\r' % (GPIO_SDA, GPIO_SCL, CODEC_ADDR, start_reg, n), 0)
        
    data = ''
    time_max = time.time() + 10.0
    while not contains_any(data, 'OK\r', 'ERROR\r') and time.time() < time_max:
        byte = MDM.readbyte()
        if byte != -1:
            data += chr(byte)

    data = data.lstrip()
    if data.startswith('#I2CRD: '):
        return data[8:data.index('\r')]
    
    return None


def config_codec():
    regs, values = zip(*sorted(config.iteritems(), key=lambda x: x[0]))
    if not write_registers(regs[0], *values[:-1]):
        return False
    
    # 'Toggle the SHDN bit whenever a configuration change is made.'
    return write_registers(0x17, 0x8a) \
        and write_registers(0x17, 0x0a) \
        and write_registers(0x17, 0x8a)

def accept_call():
    MDM.send('ATA\r', 0)
    return wait_for_response(5)


def hang_call():
    MDM.send('ATH\r', 0)
    return wait_for_response(5)

def main():
    if config_codec():
        SER2.send('Codec configured\r\n')
    else:
        SER2.send('Cannot configure the codec\r\n')
    
    data = ''
    while True:
        byte = MDM.readbyte()
        if byte == -1:
            time.sleep(0.1)
            continue
        
        data += chr(byte)
        if not '\r' in data:
            time.sleep(0.1)
            continue
        
        line, data = data.split('\r', 2)
        if 'RING' in line:
            SER2.send('ring!!!\r\n')
            accept_call()
        else:
            SER2.send(line + '\r\n')


SER2.set_speed('9600', '8N1')
SER2.send('Starting...\r\n')
try:
    main()
except BaseException, e:
    SER2.send(repr(e))


# 
# SER2.set_speed('115200', '8N1')
# 
# MDM.send('AT+CMEE=2\n', 0)
# while True:
#     MDM.send('AT+COPS=?\r', 0)
#     MDM.send('AT+CSQ\r', 0)
#     MDM.send('AT+CREG?\r', 0)
#     for line in MDM.read().split('\r\n'):
#         if line and line != 'OK':
#             SER2.send(line + '\r\n')
#     time.sleep(0.1)
