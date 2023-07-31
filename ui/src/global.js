


function getContext() {
    const urlParams = new URLSearchParams(window.location.search);

    let host = urlParams.get('host');

    if (!host || host == "") {
        host = window.location.hostname;
    }

    return {
        host: host
    }
}