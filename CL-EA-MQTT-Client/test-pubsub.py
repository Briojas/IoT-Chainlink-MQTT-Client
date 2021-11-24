import pytest
import adapter

job_run_id = '1'
device_name = 'rfidElement'
leds = 3

sub_test_data = {'id': job_run_id, 'data': {'subscribe':{'topics': "/" + device_name + "/tag", 'qos':0}}}

pub_test_data = {'id': job_run_id, 'data': {'publish': [
        {'topic': '/' + device_name + '/LEDs', 'qos': 0, 'payload': leds, 'retain': True}
]}}

a = adapter.Adapter(sub_test_data)
print(a.result)

b = adapter.Adapter(pub_test_data)
print(b.result)