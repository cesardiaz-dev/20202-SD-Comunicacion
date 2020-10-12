package co.edu.utp.isc.sd.soapclient;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;

import co.edu.utp.isc.sd.soapclient.countries.GetCountryResponse;

@SpringBootApplication
public class SoapClientApplication {

	public static void main(String[] args) {
		SpringApplication.run(SoapClientApplication.class, args);
	}

	@Bean
	CommandLineRunner lookup(CountryClient client) {
		return args -> {
			String country = "Colombia";

			if (args.length > 0) {
				country = args[0];
			}
			GetCountryResponse response = client.getCountry(country);
			System.err.println(response.getCountry().getCurrency());
		};
	}

}
