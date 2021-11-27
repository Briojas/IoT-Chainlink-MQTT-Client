// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "@chainlink/contracts/src/v0.8/ChainlinkClient.sol";
import "@chainlink/contracts/src/v0.8/ConfirmedOwner.sol";

contract MQTTClient is ChainlinkClient, ConfirmedOwner{
    using Chainlink for Chainlink.Request;
    string public data_string;
    int256 public data_int;
    //uint256 public data_float;
    uint256 public counter;
    
    address private oracle;
    bytes32 private jobId_strings;
    bytes32 private jobId_ints;
    //bytes32 private jobId_floats;
    uint256 private fee;
    
    /**
     * Network: Kovan
     * Oracle: 0xEcA7eD4a7e36c137F01f5DAD098e684882c8cEF3
     * Job IDs: below
     * Fee: 1 LINK
     */
    constructor() ConfirmedOwner(msg.sender) {
        setPublicChainlinkToken();
        oracle = 0xEcA7eD4a7e36c137F01f5DAD098e684882c8cEF3;
        jobId_strings = "18233b7ffdc146f6844750fec67b3453";
        jobId_ints =    "f485e865867047e3a6b6eefde9b3a600";
        //jobId_floats = _jobId;
        fee = 1 * LINK_DIVISIBILITY;
        counter = 0;
    }
    
    /**
     * Create a Chainlink request to publish on, or subscribe to, a topic on the MQTT brokers available in the Node's External Adapter containing string data.
     */
    function sub_string(string calldata _action, string calldata _topic, int16 _qos) public onlyOwner returns (bytes32 requestId){return pub_string(_action, _topic, _qos, "", 1);}
    function pub_string(string calldata _action, string calldata _topic, int16 _qos, string memory _payload, int16 _retain) public onlyOwner returns (bytes32 requestId) 
    {
        Chainlink.Request memory request = buildChainlinkRequest(jobId_strings, address(this), this.fulfill_string.selector);
        
        // Set the params for the external adapter
        request.add("action", _action); //options: "publish", "subscribe"
        request.add("topic", _topic);
        request.addInt("qos", _qos);
        request.add("payload", _payload);
        request.addInt("retain", _retain);
        
        // Sends the request
        return sendChainlinkRequestTo(oracle, request, fee);
    }
    function sub_int(string calldata _action, string calldata _topic, int16 _qos) public onlyOwner returns (bytes32 requestId){return pub_int(_action, _topic, _qos, 0, 1);}
    function pub_int(string calldata _action, string calldata _topic, int16 _qos, int256 _payload, int16 _retain) public onlyOwner returns (bytes32 requestId) 
    {
        Chainlink.Request memory request = buildChainlinkRequest(jobId_ints, address(this), this.fulfill_string.selector);
        
        // Set the params for the external adapter
        request.add("action", _action); //options: "publish", "subscribe"
        request.add("topic", _topic);
        request.addInt("qos", _qos);
        request.addInt("payload", _payload);
        request.addInt("retain", _retain);
        
        // Sends the request
        return sendChainlinkRequestTo(oracle, request, fee);
    }
    
    
    /**
     * Receive the response in the form of string, but convert to bytes32 for storage
     */ 
    function fulfill_string(bytes32 _requestId, string calldata _returnString) public recordChainlinkFulfillment(_requestId)
    {
        counter = counter + 1;
        data_string = _returnString;
    }
    function fulfill_int(bytes32 _requestId, int256 _returnInt) public recordChainlinkFulfillment(_requestId)
    {
        counter = counter + 1;
        data_int = _returnInt;
    }

    function get_string() public view returns (string memory){
        return data_string;
    }
    function get_int() public view returns (int256){
        return data_int;
    }

    function withdrawLink() public onlyOwner {
        LinkTokenInterface link = LinkTokenInterface(chainlinkTokenAddress());
        require(link.transfer(msg.sender, link.balanceOf(address(this))), "Unable to transfer");
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
