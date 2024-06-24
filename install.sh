if [ "$EUID" -ne 0 ]; then
	echo "Please run as root"
	exit
fi
INSTALL_LOG_FILE="use-nvidia-install.log"
UNVINSTLG() {
	echo "$@"
	echo "$@" >>$INSTALL_LOG_FILE
}
UNVINSTLG "Started installation on $(date)"

INSTALLPATH=/etc/unv
BINARYINSTALLPATH=/usr/bin

UNVINSTLG "Installing to $INSTALLPATH"
UNVINSTLG "Creating directory"

[ -d $INSTALLPATH ] || mkdir $INSTALLPATH

UNVINSTLG "Creating empty logfile"

touch $INSTALLPATH/use-nvidia.log

UNVINSTLG "Copying unv to $BINARYINSTALLPATH"

cp unv $BINARYINSTALLPATH

UNVINSTLG "Installation finished on $(date)"

echo "For the verbatim log, please check $INSTALL_LOG_FILE"
