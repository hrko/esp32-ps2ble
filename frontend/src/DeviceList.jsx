import React from "react";
import Button from "@mui/material/Button";
import ListItem from "@mui/material/ListItem";
import List from "@mui/material/List";

function DeviceList({ devices, onDelete }) {
  return (
    <List>
      {devices.map((device) => (
        <ListItem
          key={device.address}
          style={{ display: "flex", justifyContent: "space-between" }}
          disableGutters
        >
          <span>
            {device.name} ({device.address})
          </span>
          <Button
            variant="contained"
            color="secondary"
            onClick={() => onDelete(device.address, device.addressType)}
          >
            削除
          </Button>
        </ListItem>
      ))}
    </List>
  );
}

export default DeviceList;
