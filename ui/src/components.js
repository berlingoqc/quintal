import {LitElement, html, css} from 'https://cdn.jsdelivr.net/gh/lit/dist@2/core/lit-core.min.js';


class SimpleGreeting extends LitElement {
  static styles = css`p { color: blue }`;
  static properties = {
    name: {type: String}
  };

  constructor() {
    super();
  }

  render() {
    return html`<p>Hello, ${this.name}!</p>`;
  }
}


customElements.define('simple-greeting', SimpleGreeting)
