# [HeartCare](https://github.com/FlameAlpha/HeartCare.git)

<img src="../Image/heart.png" style="zoom:10%;" /> <img src="../Image/bluetooth.png" style="zoom:12%;" /> <img src="../Image/ai.png" style="zoom:12%;" />
心率检测及诊断工具

## 环境

1. 图形界面 : Qt Creator 4.8.4
2. 编译器 : Qt 5.12.2 MSVC 2017 64Bit
3. C++版本 : C++ 17

该工程的**已知依赖库**包括但不限于：

1. **SQL** <libcurl.dll，libmysql.dll，qsqlmysql.dll>
2. **python-3.7.4-embed-amd64** <sqlite3.dll，libcrypto-1_1，libssl-1_1.dll，vcruntime140.dll，python37.dll，python3.dll>
3. **Qt** <Qt5Bluetooth.dll，Qt5Charts.dll，Qt5Core.dll，Qt5Gui.dll，Qt5Sql.dll，Qt5Svg.dll，Qt5Widgets.dll>
4. **Windows** <msvcp140.dll，kernel32.dll，vcruntime140.dll，shell32.dll>
5. **Others **<libeay32.dll，libEGL.dll，libGLESV2.dll，ssleay32.dll，opengl32sw.dll>

## 工程文件树

HeartCare
    |― bluetooth service <bluetoothservice.h>
    |― bluetooth socket <socketdataprocess.h>
    |― user&device info input <keyin.h>
    |― SQL handler <handlesql.h handleinfluxdb.h>
    |― chart show
    |    |― series for HeartCare <chartseries.h>
    |    |― chart for HeartCare <chartshow.h>
    |    |― group of charts <devicecharts.cpp>
    |    |― vertivalline <vertivalline.h>
    |    |― device data <dataclass.h>
    |    |― device Info <dataclass.h>
    |    |― data show window <datashow.h>
    |― AI report
    |    |― csv writer <csvwriter.h>
    |    |― Python API <report.h>

## 软件功能树

HeartCare
    |― bluetooth service <bluetoothservice.h> : 根据MAC地址建立蓝牙通信
    |    |― scan bluetooth devices
    |    |― creat bluetooth socket
    |― bluetooth socket <socketdataprocess.h> ：接收信息后将数据发往显示界面对象
    |    |― parse data package
    |    |― receive and send data
    |    |― send data to show handler
    |― user&device info input <keyin.h> ：键入用户及设备信息
    |― SQL handler <handlesql.h> : 操作MySQL数据库
    |    |― creat table
    |    |― insert Patient&Device data  with multithreading
    |    |― inquire&insert Patient&Device info
    |― SQL handler <handleinfluxdb.h> : 操作InfluxDB数据库，未投入使用
    |    |― insert Patient&Device data
    |― series for HeartCare <chartseries.h> ：适应于该项目的几种线条类型
    |    |― AreaSeries
    |    |― BarSeries
    |    |― LineSeries
    |    |― SplineSeries
    |    |― ScatterSeries
    |― chart for HeartCare <chartshow.h> ：适应于该项目的图表
    |    |― automatically adjust range of X&Y axis
    |    |― change one point of serie
    |    |― set serie data list
    |    |― set title of chart
    |― group of charts <devicecharts.cpp> ：适应于该项目的图表集合
    |― vertivalline <vertivalline.h> ：特殊线条**竖线**
    |    |― change position
    |― device data&Info <dataclass.h> ：设备数据及信息类
    |    |― DeviceData
    |    |― DeviceInfo
    |― data show window <datashow.h> ：人机交互(数据展示)界面
    |    |― initialize all item of this user interface window
    |    |― append the new device data and change the chart data list
    |― csv writer <csvwriter.h> ：设备数据及信息类
    |    |― insert data with multithreading
    |    |― write *maxdatalength* data into a new *.csv file 
    |― Python API <report.h> ：调用Python API进行报告生成，并修改报告显示区域图表数据
    |    |― init python environment
    |    |― get report with multithreading

## 软件的运行流程

是这样的先打开这个软件之后，就会有一个蓝牙选择的软件界面，然后这个软件界面，他有扫描蓝牙设备，还有一个open，还有一个close， 反正就是几个按钮，主要用的就是scan和open 按钮， 打开蓝牙设备就是你可以点open，也可以双击它，就这个item也就是说双击它，他扫描出来这个设备双击它之后，你就会出现一个就会弹出一个界面。

就是说双击之后他就会去连接你的蓝牙设备，你有了设备之后，他会通过你的蓝牙设备去获取信息，去显示，首先你蓝牙设备设备和用户之间，它是需要怎么说呢?有一个关联关系的，也就是说病人和蓝牙设备之间有一个关联关系。首先你要把这个关联关系给它input一下，就是他API，那是Keyin文件。

然后就是说，Bluetooth service只是一个建立通信的接口。他就是说建立通讯了，创建socket data process多线程，然后每次接收到的信息都会通过socket data process API进行处理。然后你看Keyin, 当键入病人和设备信息之后，就是说API它会获取你的人就是病人和设备之间的信息。

然后就会发出一个信号，然后发我发出一个信号，让其他的handler或者一些API知道人物和设备之间的关系。然后也会说，如果没有就是说他也会存到数据库里边，如果他们不存在的话，如果存在的话，你看看是不是修改了也会修改。假如说没有修改，就是说它存在那就不修改了，就是这样的一个，然后然后剩下的就是说图形显示界面。

通过刚才的 socket data process,然后这是一个建立所谓的通信，就是说通讯之后，他就会对数据进行处理，它会进行解包等处理，之后进行了三个操作：

1. 把这些数据发送给这个图形显示界面线程，datashow就会根据数据对chart里的series进行修改或者说point list进行修改。
2. 将数据写入csv文件，文件存在`./DataLog`下。
3. 将数据转换为命令入数据库的任务队列，存入数据库。

在图形界面datashow中有一个report按钮，点击之后调用Python API生成报告，其中用于生成报告的数据是使用写入的csv文件。report生成之后会直接通过指针对图形界面进行修改。

**注意 ：某些文件使用了中文这导致无法全部使用UTF-8进行编码，需要使用system(Windows 10 中文版)编码方式对文件重载。**



