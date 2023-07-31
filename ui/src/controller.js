

let controller = null;

let keystate = {
    front: false,
    back: false,
    left: false,
    right: false,

    hasChange: false
}

// Function to handle keydown event (when a key is pressed)
function handleKeyDown(event) {
    var keyCode = event.keyCode || event.which;

    // Check which key was pressed
    switch (keyCode) {
        case 87: // W key
            keystate.front = true;
            keystate.hasChange = true;
            break;
        case 65: // A key
            keystate.left = true;
            keystate.hasChange = true;
            break;
        case 83: // S key
            keystate.back = true;
            keystate.hasChange = true;
            break;
        case 68: // D key
            keystate.right = true;
            keystate.hasChange = true;
            break;
        default:
            // Ignore other keys
            break;
    }
}

// Function to handle keyup event (when a key is released)
function handleKeyUp(event) {
    var keyCode = event.keyCode || event.which;

    // Check which key was released
    switch (keyCode) {
        case 87: // W key
            keystate.front = false;
            keystate.hasChange = true;
            break;
        case 65: // A key
            keystate.left = false;
            keystate.hasChange = true;
            break;
        case 83: // S key
            keystate.back = false;
            keystate.hasChange = true;
            break;
        case 68: // D key
            keystate.right = false;
            keystate.hasChange = true;
            break;
        default:
            // Ignore other keys
            break;
    }
}
document.addEventListener("keydown", handleKeyDown);
document.addEventListener("keyup", handleKeyUp);

window.addEventListener("gamepadconnected", (e) => {
    if (e == null || e.gamepad == null) {
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