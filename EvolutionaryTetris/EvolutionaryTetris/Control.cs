using System.IO.Ports;
namespace EvolutionaryTetris;

public class Control
{
    const int BAUD_RATE = 115200;
    const int DATA_BITS = 8;
    public required SerialPort Port {get; set;}
    
    public Control(string portName)
    {
        Port = new SerialPort(portName,  BAUD_RATE, Parity.None, DATA_BITS, StopBits.One);
    }
}