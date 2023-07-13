#include "maximtemp_driver.h"

#include <linux/iio/iio.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

#define HERE                                                                   \
    printk(KERN_DEBUG "Here is line %s:%d in file %s\n", __func__, __LINE__,   \
        __FILE__);

static int maximtemp_driver_read_raw(struct iio_dev* indio_dev,
    struct iio_chan_spec const* chan, int* val, int* val2, long mask);

static int maximtemp_driver_probe(struct spi_device*);
static int maximtemp_driver_remove(struct spi_device*);

struct maximtemp_context {
    struct spi_device* spi;
};

static int maximtemp_driver_read_raw(struct iio_dev* indio_dev,
    struct iio_chan_spec const* chan, int* val, int* val2, long mask) {

    struct maximtemp_context* context = iio_priv(indio_dev);
    u8 buffer[MAXIMTEMP_SPI_BUFFER_SIZE];
    int result = spi_read(context->spi, buffer, sizeof(buffer));
    if (result != 0) {
        HERE;
        return result;
    }

    switch (mask) {
    case IIO_CHAN_INFO_RAW:
        *val = buffer[0];
        *val2 = buffer[1];
        return IIO_VAL_INT_MULTIPLE;
    case IIO_CHAN_INFO_PROCESSED:
        maximtemp_convert_to_micros(buffer, val, val2);
        return IIO_VAL_INT_PLUS_MICRO;
    default:
        return -EINVAL;
    }
}

static const struct iio_info maximtemp_info = {
    .read_raw = maximtemp_driver_read_raw,
};

static const struct iio_chan_spec maximtemp_channels[] = {
    {
        .type = IIO_TEMP,
        .info_mask_separate
        = BIT(IIO_CHAN_INFO_RAW) | BIT(IIO_CHAN_INFO_PROCESSED),
    },
};

static int maximtemp_driver_probe(struct spi_device* spi) {
    int result;
    struct maximtemp_context* context;

    struct iio_dev* iio_device
        = devm_iio_device_alloc(&spi->dev, sizeof(struct maximtemp_context));
    if (iio_device == NULL) {
        HERE;
        return result;
    }

    iio_device->name = "maximtemp";
    iio_device->info = &maximtemp_info;
    iio_device->modes = INDIO_DIRECT_MODE;
    iio_device->channels = maximtemp_channels;
    iio_device->num_channels = ARRAY_SIZE(maximtemp_channels);

    context = iio_priv(iio_device);
    context->spi = spi;

    result = spi_setup(spi);
    if (result != 0) {
        HERE;
        return result;
    }

    result = devm_iio_device_register(&spi->dev, iio_device);
    if (result != 0) {
        HERE;
        return result;
    }

    pr_info("maximtemp_driver probe\n");
    return 0;
}

static int maximtemp_driver_remove(struct spi_device* spi) {
    pr_info("maximtemp_driver remove\n");
    return 0;
}

static const struct of_device_id maximtemp_driver_dt_ids[] = {
    {
        .compatible = "pitcer,maximtemp",
    },
};

static struct spi_driver maximtemp_driver = {
    .driver = {
        .name = "maximtemp_driver" ,
        .of_match_table = maximtemp_driver_dt_ids,
    },
    .probe = maximtemp_driver_probe,
    .remove = maximtemp_driver_remove,
};

module_spi_driver(maximtemp_driver);
MODULE_LICENSE("GPL");
