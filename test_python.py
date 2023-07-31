from pyfirmata import Arduino
import asyncio
from websockets.server import serve


MOTOR_A = 0
MOTOR_B = 1
HIGH = 1
LOW = 0


class Motor():
    def __init__(self, board, in1, in2, pwn):
        self.in1 = board.get_pin('d:{}:o'.format(in1))
        self.in2 = board.get_pin('d:{}:o'.format(in2))
        self.pwn = board.get_pin('d:{}:p'.format(pwn))


class L298N():
    def __init__(self, board, ena, in1, in2, in3, in4, enb):
        self.board = board

        self.motor = {
            0: Motor(board, in1, in2, ena),
            1: Motor(board, in3, in4, enb)
        }

    def drive_motor(self, motor_index, speed):
        self.motor[motor_index].pwn.write(speed)

    def drive_motors(self, speed):
        self.drive_motor(MOTOR_A, speed)
        self.drive_motor(MOTOR_B, speed)

    def setup_motor(self, motor_index, state1, state2):
        self.motor[motor_index].in1.write(state1)
        self.motor[motor_index].in2.write(state2)

    def setup_motors(self, state1, state2, state3, state4):
        self.setup_motor(MOTOR_A, state1, state2)
        self.setup_motor(MOTOR_B, state3, state4)

    def forward(self, speed, delay_time):
        #self.setup_motors(HIGH, LOW, HIGH, LOW)
        self.setup_motor(MOTOR_A, HIGH, LOW)
        #self.setup_motors(HIGH, LOW, LOW, LOW)
        self.drive_motor(MOTOR_A, speed)
        self.board.pass_time(delay_time)

    def backward(self, speed, delay_time):
        #self.setup_motors(LOW, HIGH, LOW, LOW)
        self.setup_motor(MOTOR_A, LOW, HIGH)
        self.drive_motor(MOTOR_A, speed)
        self.board.pass_time(delay_time)

    def full_stop(self, delay_time):
        self.setup_motors(LOW, LOW, LOW, LOW)
        self.drive_motors(0)
        self.board.pass_time(delay_time)

    def stop(self, motor_index, delay_time):
        self.setup_motor(motor_index, LOW, LOW)
        self.drive_motor(motor_index, 0)
        self.board.pass_time(delay_time)

    def turn_left(self, speed, delay_time):
        self.setup_motor(MOTOR_B, LOW, HIGH)
        self.drive_motor(MOTOR_B, speed)
        self.board.pass_time(delay_time)

    def turn_right(self, speed, delay_time):
        self.setup_motor(MOTOR_B, HIGH, LOW)
        self.drive_motor(MOTOR_B, speed)
        self.board.pass_time(delay_time)

board = Arduino('/dev/ttyACM0')
ena = 10
enb = 11
in1 = 5
in2 = 4
in3 = 6
in4 = 7

motor = L298N(board, ena, in1, in2, in3, in4, enb)
# Forward with speed 70%
motor.setup_motors(LOW, LOW, LOW, LOW)


async def echo(websocket):
    async for message in websocket:
        print(message);
        elements = message.split(',');
        if len(elements) == 8:
            l_y = int(elements[1])
            l_x = int(elements[0])

            power = int(elements[4])
            reverse = int(elements[5])

            pwm_motor_a = float(elements[6]);
            pwm_motor_b = float(elements[7]);

            if reverse:
                print("reverse")
                motor.backward(pwm_motor_a, 0.0)
            elif power:
                print("forward")
                motor.forward(pwm_motor_a, 0.0)
            else:
                print("stop")
                motor.stop(MOTOR_A, 0.0)
            
            if l_x > 5:
                print("right")
                motor.turn_right(pwm_motor_b, 0.0)
            elif l_x < -5:
                print("left")
                motor.turn_left(pwm_motor_b, 0.0)
            else:
                print("stop steering")
                motor.stop(MOTOR_B, 0.0)


async def main():
    async with serve(echo, "0.0.0.0", 9002):
        await asyncio.Future()  # run forever

if __name__ == '__main__':
    asyncio.run(main())