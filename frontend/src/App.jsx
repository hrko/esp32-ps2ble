import React, { useState, useEffect } from 'react';
import axios from 'axios';
import DeviceList from './DeviceList';

function App() {
  const [devices, setDevices] = useState([]);

  useEffect(() => {
    fetchDevices();
  }, []);

  const fetchDevices = async () => {
    try {
      const response = await axios.get('http://192.168.10.230/api/bonded-devices');
      setDevices(response.data.bondedDevices);
    } catch (error) {
      console.error('Error fetching devices:', error);
    }
  };

  const deleteDevice = async (address, addressType) => {
    try {
      const response = await axios.post('http://192.168.10.230/api/bonded-devices/delete', { address, addressType });
      if (response.data.deleted) {
        fetchDevices(); // リストを更新
      } else {
        alert(`Error: ${response.data.message}`);
      }
    } catch (error) {
      console.error('Error deleting device:', error);
    }
  };

  return (
    <div className="App">
      <h1>ペアリング済みデバイス</h1>
      <button onClick={fetchDevices}>デバイスリストを更新</button>
      <DeviceList devices={devices} onDelete={deleteDevice} />
    </div>
  );
}

export default App;
