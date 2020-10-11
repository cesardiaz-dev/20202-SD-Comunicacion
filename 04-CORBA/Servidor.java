import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;

public class Servidor {
    public static void main(String[] args) {
        try {
            // Iniciar el ORB 
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);

            // Encontrar la referencia del POA raiz 
            POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
            // Activar el POA
            rootPOA.the_POAManager().activate();

            // Crear el objeto implementacion y lo registra en el ORB  
            CalculadoraImpl calcImpl = new CalculadoraImpl();
            
            // Registrarlo en el POA
            org.omg.CORBA.Object ref = rootPOA.servant_to_reference(calcImpl);
            distribuidos.Calculadora calc = distribuidos.CalculadoraHelper.narrow(ref);

            // Obtener la raiz del contexto de llamado
            // NameService que invoca el servicio de nombres
            org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
            // Usar NamingContextExt el cual es parte de la Especificacion de Servicio de Nombres Interoperables (INS)
            NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

            //Enlazo la referencia del objeto al servicio de nombres
            String nombre = "Calculadora";
            NameComponent path[] = ncRef.to_name(nombre);
            ncRef.rebind(path, calc);

            System.out.println("Servidor listo y esperando...");

            // Esperar llamadas
            orb.run();
        }catch(Exception e) {
            e.printStackTrace();
        }
        System.out.println("Servidor ha cerrado.");
    }
}
