

let controller;

window.addEventListener("gamepadconnected", (e) => {
    if (e == null) {
        return;
    }
    console.log(
        "Gamepad connected at index %d: %s. %d buttons, %d axes.",
        e.gamepad.index,
        e.gamepad.id,
        e.gamepad.buttons.length,
        e.gamepad.axes.length,
    );

    if (!this.controller) {
        controller = e.gamepad;
    } else {
        console.log('ignoring already have a gamepad');
    }
});

window.addEventListener("gamepaddisconnected", (e) => {
    controller = null;
});


const isControllerConnected = () => controller != undefined || controller != null;