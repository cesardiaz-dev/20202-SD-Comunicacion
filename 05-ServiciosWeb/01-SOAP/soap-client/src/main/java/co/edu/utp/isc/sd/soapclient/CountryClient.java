package co.edu.utp.isc.sd.soapclient;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.springframework.ws.client.core.support.WebServiceGatewaySupport;
import org.springframework.ws.soap.client.core.SoapActionCallback;

import co.edu.utp.isc.sd.soapclient.countries.GetCountryRequest;
import co.edu.utp.isc.sd.soapclient.countries.GetCountryResponse;

public class CountryClient extends WebServiceGatewaySupport {

    private static final Logger log = LoggerFactory.getLogger(CountryClient.class);

    public GetCountryResponse getCountry(String country) {

        GetCountryRequest request = new GetCountryRequest();
        request.setName(country);

        log.info("Requesting location for " + country);

        GetCountryResponse response = (GetCountryResponse) getWebServiceTemplate().marshalSendAndReceive(
                "http://localhost:8080/ws", request,
                new SoapActionCallback("http://isc.utp.edu.co/sd/soapserver/countries/GetCountryRequest"));

        return response;
    }
}
