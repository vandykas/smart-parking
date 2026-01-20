const ws = new WebSocket("wss://smart-parking-production.up.railway.app/ws/parking");
const parkingData = {
    ppag: {},
    ged9: {}
};

updateDisplay("ged9");
updateDisplay("ppag");

ws.onmessage = (event) => {
    try {
        const data = JSON.parse(event.data);
        // Topic example: unpar/ged9/b1/a2
        const parts = data.topic.split("/");

        // Ensure we have enough parts [unpar, gedung, lantai, slot]
        if (parts.length < 4) return;

        const gedung = parts[1]; // ppag or ged9
        const slotKey = parts[2] + "_" + parts[3]; // e.g. b1_a2

        const distance = parseInt(data.mqtt, 10);
        // Logic: Distance < 200 is Occupied (Red). >= 200 is Empty (Green/Available).
        const isAvailable = distance >= 200;

        if (!parkingData[gedung]) {
            parkingData[gedung] = {};
        }

        parkingData[gedung][slotKey] = isAvailable;

        updateDisplay(gedung);

    } catch (e) {
        console.error("Error processing MQTT message", e);
    }
};

function updateDisplay(gedung) {
    const slots = parkingData[gedung];
    let count = 0;

    // Count how many slots are true (Available)
    for (const key in slots) {
        if (slots[key] === true) {
            count++;
        }
    }

    let elementId = "";
    if (gedung === 'ppag') {
        elementId = 'avail-ppag';
    } else if (gedung === 'ged9') {
        elementId = 'avail-ged9';
    }

    if (elementId) {
        const el = document.getElementById(elementId);
        if (el) {
            el.textContent = `${count} Available`;
        }
    }
}
