from rest_framework import serializers
from .models import SensorReading

class SensorReadingSerializer(serializers.ModelSerializer):
    class Meta:
        model = SensorReading
        fields = ['id', 'timestamp', 'left_distance', 'right_distance', 'back_distance', 'alert_level', 'device_id'] 