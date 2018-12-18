#include "packetparser.h"

PacketParser::PacketParser()
{
    //
}

PacketParser::~PacketParser()
{
    //
}

void PacketParser::setCalibrationPacket(Packet zeroOffset)
{
    m_zeroOffset = zeroOffset;
}

void PacketParser::setPacket(QString packet)
{
    m_parsePacket(packet);
}

void PacketParser::setPacket(QByteArray packet)
{
    m_parsePacket(packet);
}

void PacketParser::m_parsePacket(QByteArray packet)
{
    // Packet size 16 bytes with packet delimiter 0x7F 0xFF at the end
    if(packet.size() >= 16){
        if(packet[14] != 0x7F && packet[15] != 0xFF) return;
    } else {
        return;
    }
    m_parsedPacket.ax((fromTwoBytes(packet[0], packet[1]) * 1.0));
    m_parsedPacket.ay((fromTwoBytes(packet[2], packet[3]) * 1.0));
    m_parsedPacket.az((fromTwoBytes(packet[4], packet[5]) * 1.0));

    m_parsedPacket.gx((fromTwoBytes(packet[6], packet[7]) * 1.0));
    m_parsedPacket.gy((fromTwoBytes(packet[8], packet[9]) * 1.0));
    m_parsedPacket.gz((fromTwoBytes(packet[10], packet[11]) * 1.0));

    double temp = 0;
    temp = fromTwoBytes(packet[12], packet[13]) * 1.0;
    temp = temp / 340.00 + 36.53;
    m_parsedPacket.temp(temp);

    m_parsedPacket += m_zeroOffset;
}

int16_t PacketParser::fromTwoBytes(uint8_t val1, uint8_t val2)
{
    int16_t output = 0;
    output = val1 & 0x00FF;
    output |= ((val2 & 0x00FF) << 8);
    return output;
}

void PacketParser::m_parsePacket(QString packet)
{
    QStringList parsedPacket = packet.split(" ");
    m_parsedPacket.ax(parsedPacket[0].toInt() * 1.0);
    m_parsedPacket.ay(parsedPacket[1].toInt() * 1.0);
    m_parsedPacket.az(parsedPacket[2].toInt() * 1.0);
    m_parsedPacket.gx(parsedPacket[3].toInt() * 1.0);
    m_parsedPacket.gy(parsedPacket[4].toInt() * 1.0);
    m_parsedPacket.gz(parsedPacket[5].toInt() * 1.0);
    m_parsedPacket.temp(parsedPacket[6].toDouble());

    m_parsedPacket += m_zeroOffset;
}

Packet PacketParser::getPacket()
{
    switch(m_filterType){
        case RAW: {
            return m_parsedPacket;
            break;
        }
        case NONE: {
            return toAngle(m_parsedPacket);
            break;
        }
        case KALMAN: {
            return kalman(toAngle(m_parsedPacket));
            break;
        }
        case COMPLEMENTARY: {
            return complementary(toAngle(m_parsedPacket));
            break;
        }
    }
    return m_parsedPacket;
}

Packet PacketParser::kalman(Packet packet)
{
    Packet filteredPacket;
    filteredPacket.ax( m_kalmanFilterAx.filter( packet.ax() ) );
    filteredPacket.ay( m_kalmanFilterAy.filter( packet.ay() ) );
    filteredPacket.az( m_kalmanFilterAz.filter( packet.az() ) );

    filteredPacket.gx( m_kalmanFilterGx.filter( packet.gx() ) );
    filteredPacket.gy( m_kalmanFilterGy.filter( packet.gy() ) );
    filteredPacket.gz( m_kalmanFilterGz.filter( packet.gz() ) );

    filteredPacket.temp(packet.temp());

    return filteredPacket;
}

void PacketParser::setKalmanCoefficient(double coeff)
{
    m_kalmanFilterAx.setCoefficient(coeff);
    m_kalmanFilterAy.setCoefficient(coeff);
    m_kalmanFilterAz.setCoefficient(coeff);
    m_kalmanFilterGx.setCoefficient(coeff);
    m_kalmanFilterGy.setCoefficient(coeff);
    m_kalmanFilterGz.setCoefficient(coeff);
}

Packet PacketParser::complementary(Packet packet)
{
    Packet filteredPacket;

    filteredPacket.ax( m_complementaryFilterX.filter( packet.ax(), packet.gx(), m_gyroAngleX ) );
    filteredPacket.ay( m_complementaryFilterY.filter( packet.ay(), packet.gy(), m_gyroAngleY ) );
    filteredPacket.az( m_complementaryFilterZ.filter( packet.az(), packet.gz(), m_gyroAngleZ ) );

    filteredPacket.gx(0);
    filteredPacket.gy(0);
    filteredPacket.gz(0);

    filteredPacket.temp(packet.temp());
    return filteredPacket;
}

void PacketParser::setComplementaryCoefficient(double coeff)
{
    m_complementaryFilterX.setCoefficient(coeff);
    m_complementaryFilterY.setCoefficient(coeff);
    m_complementaryFilterZ.setCoefficient(coeff);
}

void PacketParser::setDeltaTime(double dt)
{
    m_complementaryFilterX.setDeltaTime(dt);
    m_complementaryFilterY.setDeltaTime(dt);
    m_complementaryFilterZ.setDeltaTime(dt);
}

Packet PacketParser::toAngle(Packet packet)
{
    Packet processedPacket;

    processedPacket.ax( accelDataToAngle(packet.ax()) );
    processedPacket.ay( accelDataToAngle(packet.ay()) );
    processedPacket.az( accelDataToAngle(packet.az()) );

    gyroDataToAngle(packet.gx(), m_gyroAngleX);
    gyroDataToAngle(packet.gy(), m_gyroAngleY);
    gyroDataToAngle(packet.gz(), m_gyroAngleZ);

    processedPacket.gx( m_gyroAngleX );
    processedPacket.gy( m_gyroAngleY );
    processedPacket.gz( m_gyroAngleZ );

    //processedPacket = packet;
    //
    //DistanceIntegrator di;
    //di.processPacket(processedPacket);

    processedPacket.temp(packet.temp());

    return processedPacket;
}

double PacketParser::accelDataToAngle(double accelData)
{
    // 16g - 2048 = 1G
    double angle = 0.0;
    double gValue = accelData / m_1G;

    if(gValue > 1.0){
        gValue = 1.0;
    } else if(gValue < -1.0) {
        gValue = -1.0;
    }

    if(gValue >= 0){
        angle = 90 - m_toDeg * acos(gValue);
    }else{
        angle = m_toDeg * acos(-gValue) - 90;
    }

    return angle;
}

void PacketParser::gyroDataToAngle(double gyroData, double &resultGyroAngle)
{
    resultGyroAngle = resultGyroAngle + (gyroData / 16.384) * m_dt / 1000.0;
}

void PacketParser::setFilter(FilterType type)
{
    m_filterType = type;
}

FilterType PacketParser::getCurrentFilter()
{
    return m_filterType;
}

QString PacketParser::getString()
{
    QString str = "";
    str += QString::number(m_parsedPacket.ax()) + " ";
    str += QString::number(m_parsedPacket.ay()) + " ";
    str += QString::number(m_parsedPacket.az()) + " ";

    str += QString::number(m_parsedPacket.gx()) + " ";
    str += QString::number(m_parsedPacket.gy()) + " ";
    str += QString::number(m_parsedPacket.gz()) + " ";

    str += QString::number(m_parsedPacket.temp()) + " ";

    return str;
}


