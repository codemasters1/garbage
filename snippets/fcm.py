import sys
from pyfcm import FCMNotification
push_service = FCMNotification(api_key="x:x-x-x")
registration_id = "x:x-x-x"
message_title = "Uber update"
message_body = "Hi john, your customized news for today is ready"
result = push_service.notify_single_device(registration_id=registration_id, message_title=message_title, message_body=sys.argv[1])

print(result)