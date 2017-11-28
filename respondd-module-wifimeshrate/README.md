# respondd-module-wifimeshrate

This module adds the txrate of wifi meshlinks to the respondd neighbours provider.
The module will add a `txrate` metric to each neighbour in the list.
Example output:

```json
{
  "wifi": {
    "02:00:00:00:00:00": {
      "neighbours": {
        "02:ff:ff:ff:ff:ff": {
          "inactive": 50,
          "noise": -95,
          "signal": -78,
          "txrate": 6500
        }
      }
    }
  }
}
```

The `txrate` is represented in kilobits per second.

Combination of the txrate as seen from both neigbours of a link allow for an
assessment of overall link quality.
