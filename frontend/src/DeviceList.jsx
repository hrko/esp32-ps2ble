import React from 'react';

function DeviceList({ devices, onDelete }) {
  return (
    <div>
      {devices.map(device => (
        <div key={device.address}>
          <span>{device.name} ({device.address})</span>
          <button onClick={() => onDelete(device.address, device.addressType)}>削除</button>
        </div>
      ))}
    </div>
  );
}

export default DeviceList;
