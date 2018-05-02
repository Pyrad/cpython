import spam

def adder(x, y):
    return x + y

spam.setcallback(adder)
spam.testcallback(3, 4)


spam.parrot(0)
spam.parrot(voltage=1, state='a smile', action='shout', type='yellow')
spam.parrot(2, type='green', state='cry', action='run')
spam.parrot(3)
