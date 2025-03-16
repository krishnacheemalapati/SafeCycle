from django.db import models

class SensorReading(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    left_distance = models.FloatField()
    right_distance = models.FloatField()
    back_distance = models.FloatField()
    alert_level = models.CharField(max_length=10)  # 'RED', 'YELLOW', or 'NONE'
    device_id = models.CharField(max_length=50)

    class Meta:
        ordering = ['-timestamp']

    def __str__(self):
        return f"Reading from {self.device_id} at {self.timestamp}" 