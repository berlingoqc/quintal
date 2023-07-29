


function getContext() {
    const urlParams = new URLSearchParams(window.location.search);

    const host = urlParams.get('host');

    return {
        host: host
    }
}