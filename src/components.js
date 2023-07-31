import { LitElement, html, css } from 'https://cdn.jsdelivr.net/gh/lit/dist@2/core/lit-core.min.js';


class Element extends LitElement {

  constructor() {
    super();
    this.context = getContext();
  }

  createRenderRoot() {
    return this;
  }
}


class MissingHost extends Element {

  render() {
    return html`
      <p>missing host in query params</p> 
    `;
  }

}

class VideoPlayer extends Element {

  render() {
    return html`
    <img src="http://${this.context.host}:8080/stream">
    `;
  }

}

class GamePagePreview extends Element {
  static properties = {
    power: { type: Number }
  };
  

  constructor() {
    super();

    window.addEventListener("gamepadconnected", () => this.requestUpdate());
    window.addEventListener("gamepaddisconnected", () => this.requestUpdate());

    this.socket = new WebSocket("ws://" + this.context.host + ":9002");

    this.socket.addEventListener("open", (event) => {
      this.socket.send("Hello Server!");
    });
  }

  render() {
    setTimeout(() => {
      this.requestUpdate();
    }, 50);
    if (controller != null) {
      const axes = controller.axes.map(axe => Math.round(axe * 100));

      axes.push(controller.buttons[0].pressed ? 1 : 0, controller.buttons[1].pressed ? 1 : 0, 255, 255);

      this.socket.send(axes.join(","));
    }
    if (keystate.hasChange) {
      const payload = [
        keystate.right ? -100 : (keystate.left) ? 100 : 0,
        0,
        0,
        0,
        keystate.front ? 1 : 0,
        keystate.back ? 1 : 0,
        this.power / 100.0,
        this.power / 100.0
      ];

      this.socket.send(payload.join(","));

      console.log('sending keystroke ' + this.power);
    }

  }
}

class VehiculeControlPage extends Element {

  render() {
    return html`

      <div hx-ext="client-side-templates">
        <div hx-get="http://${this.context.host}:8080/state" hx-swap="innerHTML" hx-target="#content" mustache-template="foo" hx-trigger="load"></div>
  
        <p id="content">
          Loading pi
        </p>
  
        <template id="foo">
          <video-player></video-player>

          <p> {{ok}} </p>

          <div x-data="{ value: 50 }">
            <div>
              <input type="range" min="0" max="100" x-model="value" x-bind:value="value" x-on:input="value = $event.target.valueAsNumber">
              <span x-text="value"></span>
            </div>
            <gamepad-preview x-bind:power="value"></gamepad-preview>
          </div>
        </template>
      </div>
    `
  }
}


class MainApp extends Element {
  static styles = css`p { color: blue }`;
  static properties = {
    name: { type: String }
  };

  constructor() {
    super();

    this.context = getContext();
  }

  render() {
    if (this.context.host == null || this.context.host == "") {
      return html`<missing-host></missing-host>`;
    } else {
      return html`
        <p>Hello, ${this.name}!</p>

        <vehicule-control-page></vehicule-control-page>
      `;
    }
  }

  createRenderRoot() {
    return this;
  }
}



customElements.define('gamepad-preview', GamePagePreview)
customElements.define('video-player', VideoPlayer)
customElements.define('missing-host', MissingHost)
customElements.define('vehicule-control-page', VehiculeControlPage)
customElements.define('main-app', MainApp)
