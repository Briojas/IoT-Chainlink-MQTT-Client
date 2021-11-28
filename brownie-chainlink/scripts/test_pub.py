from brownie import MQTTClient, config, network
from scripts.helpful_scripts import fund_with_link, get_account


def main():
    account = get_account()
    mqtt_client = MQTTClient[-1]
    print(mqtt_client.address)
    tx = fund_with_link(
        mqtt_client.address, amount=config["networks"][network.show_active()]["fee"]
    )
    tx.wait(1)
    print(mqtt_client.address)
    request_tx = mqtt_client.pub_int(
        'publish',
        '/rfidElement/LEDs',
        2,
        11,
        1,
        {"from": account}
    )
    request_tx.wait(1)
    # print(mqtt_client.get_string())
