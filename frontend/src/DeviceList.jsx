import React from "react";
import Button from "@mui/material/Button";
import ListItem from "@mui/material/ListItem";
import List from "@mui/material/List";
import ListItemText from "@mui/material/ListItemText";
import ListItemIcon from "@mui/material/ListItemIcon";
import Dialog from "@mui/material/Dialog";
import DialogActions from "@mui/material/DialogActions";
import DialogContent from "@mui/material/DialogContent";
import DialogContentText from "@mui/material/DialogContentText";
import DialogTitle from "@mui/material/DialogTitle";
import Chip from "@mui/material/Chip";
import Stack from "@mui/material/Stack";
import BluetoothConnectedOutlinedIcon from "@mui/icons-material/BluetoothConnectedOutlined";
import DeviceUnknownOutlinedIcon from "@mui/icons-material/DeviceUnknownOutlined";
import MouseOutlinedIcon from "@mui/icons-material/MouseOutlined";
import KeyboardOutlinedIcon from "@mui/icons-material/KeyboardOutlined";

function ConnectedChip() {
  return (
    <Chip
      label="接続済み"
      variant="outlined"
      size="small"
      icon={<BluetoothConnectedOutlinedIcon />}
    />
  );
}

function DeviceAppearanceIcon({ appearance }) {
  switch (appearance) {
    case "Generic HID":
      return <DeviceUnknownOutlinedIcon fontSize="large" />;
    case "Keyboard":
      return <KeyboardOutlinedIcon fontSize="large" />;
    case "Mouse":
      return <MouseOutlinedIcon fontSize="large" />;
    default:
      return <DeviceUnknownOutlinedIcon fontSize="large" />;
  }
}

function SanitizedDeviceName(name) {
  if (name === "") {
    return "(No name)";
  } else {
    return name;
  }
}

function DeviceList({ devices, onDelete }) {
  return (
    <List>
      {devices.map((device) => (
        <ListItem
          key={device.address}
          style={{ display: "flex", justifyContent: "space-between" }}
          disableGutters
        >
          <ListItemIcon>
            <DeviceAppearanceIcon appearance={device.appearance} />
          </ListItemIcon>
          <ListItemText
            primary={SanitizedDeviceName(device.name)}
            secondary={device.address}
          />
          <Stack direction="row" spacing={3} alignItems={"center"}>
            {device.isConnected && <ConnectedChip />}
            <DeleteDeviceButton
              address={device.address}
              addressType={device.addressType}
              onDelete={onDelete}
            ></DeleteDeviceButton>
          </Stack>
        </ListItem>
      ))}
    </List>
  );
}

function DeleteDeviceButton({ address, addressType, onDelete }) {
  const [open, setOpen] = React.useState(false);
  const handleClickOpen = () => {
    setOpen(true);
  };
  const handleClose = () => {
    setOpen(false);
  };

  return (
    <React.Fragment>
      <Button variant="contained" color="secondary" onClick={handleClickOpen}>
        削除
      </Button>
      <Dialog
        open={open}
        onClose={handleClose}
        aria-labelledby="alert-dialog-title"
        aria-describedby="alert-dialog-description"
      >
        <DialogTitle id="alert-dialog-title">{"ペアリング解除"}</DialogTitle>
        <DialogContent>
          <DialogContentText id="alert-dialog-description">
            デバイスを削除しますか？
          </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={handleClose} color="primary">
            キャンセル
          </Button>
          <Button
            onClick={() => {
              onDelete(address, addressType);
              handleClose();
            }}
            color="primary"
            autoFocus
          >
            削除
          </Button>
        </DialogActions>
      </Dialog>
    </React.Fragment>
  );
}

export default DeviceList;
