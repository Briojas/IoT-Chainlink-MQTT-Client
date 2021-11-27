// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "@chainlink/contracts/src/v0.8/ChainlinkClient.sol";

contract MQTTClient is ChainlinkClient {
    using Chainlink for Chainlink.Request;
    bytes32 public data_string;
    uint16 public data_int;
    uint256 public data_float;
    
    address private oracle;
    bytes32 private jobId_strings;
    bytes32 private jobId_floats;
    bytes32 private jobId_ints;
    uint256 private fee;
    
    /**
     * Network: Kovan
     * Oracle: 0xEcA7eD4a7e36c137F01f5DAD098e684882c8cEF3
     * Job IDs: below
     * Fee: 0.1 LINK
     */
    constructor() {
        setPublicChainlinkToken();
        oracle = 0xEcA7eD4a7e36c137F01f5DAD098e684882c8cEF3;
        jobId_strings = "8f84c969067c45d8ad2a35dae1e5b02d";
        //jobId_floats = _jobId;
        //jobId_ints = _jobId;
        fee = 0.1 * 10 ** 18;
    }
    
    /**
     * Create a Chainlink request to publish on, or subscribe to, a topic on the MQTT brokers available in the Node's External Adapter containing string data.
     */
    function sub_string(string calldata _action, string calldata _topic, string calldata _qos) public returns (bytes32 requestId){return pub_string(_action, _topic, _qos, "", 1);}
    function pub_string(string calldata _action, string calldata _topic, string calldata _qos, string memory _payload, int16 _retain) public returns (bytes32 requestId) 
    {
        Chainlink.Request memory request = buildChainlinkRequest(jobId_strings, address(this), this.fulfill_string.selector);
        
        // Set the params for the external adapter
        request.add("action", _action); //options: "publish", "subscribe"
        request.add("topic", _topic);
        request.add("qos", _qos);
        request.add("payload", _payload);
        request.addInt("retain", _retain);
        
        // Sends the request
        return sendChainlinkRequestTo(oracle, request, fee);
    }
    function get_string() public view returns (bytes32){
        return data_string;
    }
    
    /**
     * Receive the response in the form of string, but convert to bytes32 for storage
     */ 
    function fulfill_string(bytes32 _requestId, string calldata _returnString) public recordChainlinkFulfillment(_requestId)
    {
        data_string = stringToBytes32(_returnString);
    }
    function stringToBytes32(string memory source) public pure returns (bytes32 result) {
        bytes memory tempEmptyStringTest = bytes(source);
        if (tempEmptyStringTest.length == 0) {
            return 0x0;
        }

        assembly {
            result := mload(add(source, 32))
        }
    }
}
