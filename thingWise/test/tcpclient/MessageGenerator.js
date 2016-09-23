var MessageGenerator = {
    string: function(length) {
        var message = [];
        var FILL_UNIT = '1';
        for (var i = 0; i < length; i++) {
            message.push(FILL_UNIT);
        }
        return message.join('');
    }
};

module.exports = MessageGenerator;
