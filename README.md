# Labs SO2

## Requirements

### Bochs 2.6.7

1. Download

```bash
wget https://sourceforge.net/projects/bochs/files/bochs/2.6.7/bochs-2.6.7.tar.gz \
  -O bochs-2.6.7.tar.gz
tar zxf bochs-2.6.7.tar.gz
```

2. Compile without internal debugger
   
```bash
cd bochs-2.6.7
./configure
make
sudo make install
```

3. Compile with internal debugger

```bash
./configure --enable-debugger --prefix=/usr/local/bochs_nogdb
make
sudo make install # installs on /usr/local/bochs_nogdb/bin
sudo ln -s /usr/local/bochs_nogdb/bin/bochs /usr/local/bin/bochs_nogdb
```