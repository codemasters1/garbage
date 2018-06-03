from imaplib import IMAP4_SSL

imap = IMAP4_SSL('xx')
imap.login('x', 'x')
imap.select()

typ, data = imap.search(None, 'ALL')
print(typ, data)
for num in data[0].split():
    typ, data = imap.fetch(num, '(RFC822)')
    print('Message %s\n%s\n' % (num, data[0][1]))
imap.close()
imap.logout()
