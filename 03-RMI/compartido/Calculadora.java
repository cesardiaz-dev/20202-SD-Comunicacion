package compartido;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Calculadora extends Remote {
    double suma(double num1, double num2) throws RemoteException;
}
