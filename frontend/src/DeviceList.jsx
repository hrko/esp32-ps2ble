import React from "react";
import Button from "@mui/material/Button";
import ListItem from "@mui/material/ListItem";
import List from "@mui/material/List";
import ListItemText from "@mui/material/ListItemText";
import Dialog from "@mui/material/Dialog";
import DialogActions from "@mui/material/DialogActions";
import DialogContent from "@mui/material/DialogContent";
import DialogContentText from "@mui/material/DialogContentText";
import DialogTitle from "@mui/material/DialogTitle";
import Chip from "@mui/material/Chip";
import Stack from "@mui/material/Stack";
import BluetoothConnectedRoundedIcon from "@mui/icons-material/BluetoothConnectedRounded";

function DeviceList({ devices, onDelete }) {
  return (
    <List>
      {devices.map((device) => (
        <ListItem
          key={device.address}
          style={{ display: "flex", justifyContent: "space-between" }}
          disableGutters
        >
          <ListItemText primary={device.name} secondary={device.address} />
          <Stack direction="row" spacing={3} alignItems={"center"}>
            <Chip
              label="接続済み"
              variant="outlined"
              size="small"
              icon={<BluetoothConnectedRoundedIcon />}
            />
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
